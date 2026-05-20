// demo5.cpp

#include <atomic>
#include <optional>

struct Job
{
    int id;
    int data;
};

// (is [not quite] correct)
// Unbounded lock free queue with push and non-blocking try_pop
//
// Avoids ABA using generation counter
// Avoids UAF by reusing nodes instead of deallocating them
class JobQueue5
{
    // alias for std::memory_order
    using stdmo = std::memory_order;

    // A node is a dummy node if its next pointer is set to QUEUE_END
    // We use the next ptr to establish the synchronizes-with relationship
    // next is in charge of "releasing" job
    struct Node
    {
        std::atomic<Node*> next = QUEUE_END;
        Job job;
    };

    // we use these as sentinel values.
    static inline Node* const QUEUE_END = nullptr;
    static inline Node* const STACK_END = QUEUE_END + 1;

    struct alignas(16) GenNodePtr
    {
        Node* node;
        uintptr_t gen;
    };

    static_assert(std::atomic<GenNodePtr>::is_always_lock_free);

    alignas(64) std::atomic<Node*> m_queue_back;              // producer end
    alignas(64) std::atomic<GenNodePtr> m_queue_front;        // consumer end
    alignas(64) std::atomic<GenNodePtr> m_recycled_stack_top; // recycled node stack

public:
    // Queue starts with a dummy node
    JobQueue5() //
        : m_queue_back(new Node())
        , m_queue_front(GenNodePtr { m_queue_back.load(stdmo::relaxed), 1 })
        , m_recycled_stack_top(GenNodePtr { STACK_END, 1 })
    {
    }

    ~JobQueue5()
    {
        // Assumption: no other threads are accessing the job queue
        Node* cur_node = m_queue_front.load(stdmo::relaxed).node;
        while(cur_node != QUEUE_END)
        {
            Node* next = cur_node->next;
            delete cur_node;

            cur_node = next;
        }

        // we need to clean up the recycled nodes as well
        cur_node = m_recycled_stack_top.load(stdmo::relaxed).node;
        while(cur_node != STACK_END)
        {
            Node* next = cur_node->next;
            delete cur_node;

            cur_node = next;
        }
    }

    // either get a node from the recycling stack if we have some,
    // or allocate a new one if we don't.
    Node* get_recycled_node_or_allocate_new()
    {
        GenNodePtr old_stack_top = m_recycled_stack_top.load(stdmo::relaxed);
        while(true)
        {
            if(old_stack_top.node == STACK_END)
            {
                return new Node();
            }

            // here: use **acquire**. synchronise with the release-store of
            // node->next in `add_node_to_recycling_stack`
            Node* cur_stack_next = old_stack_top.node->next.load(stdmo::acquire);
            // the rest is the same...

            GenNodePtr new_stack_top { cur_stack_next, old_stack_top.gen + 1 };

            if(m_recycled_stack_top.compare_exchange_weak( //
                   old_stack_top,                          //
                   new_stack_top,                          //
                   stdmo::relaxed))
            {
                // successfully got a node from the recycling centre
                return old_stack_top.node;
            }
        }
    }

    // Put node in recycling centre
    void add_node_to_recycling_stack(Node* node)
    {
        GenNodePtr old_stack_top = m_recycled_stack_top.load(stdmo::relaxed);
        while(true)
        {
            // here: use **release**. synchronise with the acquire-load of
            // node->next in `get_recycled_node_or_allocate_new`
            node->next.store(old_stack_top.node, stdmo::release);
            // the rest is the same...
            GenNodePtr new_stack_top { node, old_stack_top.gen + 1 };

            if(m_recycled_stack_top.compare_exchange_weak( //
                   old_stack_top,                          //
                   new_stack_top,                          //
                   stdmo::relaxed))
            {
                break;
            }
        }
    }


public:
    void push(Job job)
    {
        Node* new_dummy = get_recycled_node_or_allocate_new();
        new_dummy->next.store(QUEUE_END, stdmo::relaxed);

        Node* work_node = m_queue_back.exchange(new_dummy, stdmo::acq_rel);

        work_node->job = job;
        work_node->next.store(new_dummy, stdmo::release);
    }

    std::optional<Job> try_pop()
    {
        // most of it is the same...

        // Splice node from the front of queue, but only if it's not dummy
        // Successfully splicing a node establishes global order of pops

        GenNodePtr old_front = m_queue_front.load(stdmo::acquire);
        while(true)
        {
            // this part is similar -- just need an additional `.node` get the
            // node out from the GenNodePtr
            Node* old_front_next = old_front.node->next.load(stdmo::acquire);
            if(old_front_next == QUEUE_END)
                return std::nullopt;

            // note that the generation is strictly increasing
            GenNodePtr new_front { old_front_next, old_front.gen + 1 };

            // this part is also similar, except we CAS with the GenNodePtr instead
            // of just a simple Node*.
            if(m_queue_front.compare_exchange_weak(old_front, //
                   new_front, stdmo::acq_rel))
            {
                // Node now belongs to us
                break;
            }
        }

        Job job = old_front.node->job;
        add_node_to_recycling_stack(old_front.node);

        return job;
    }
};

#include <thread>
#include <barrier>
#include <cstdio>
#include <cstdint>

int main()
{
    JobQueue5 queue;
    auto barrier = std::barrier(3);

    auto producer1 = std::thread([&queue, &barrier]() {
        barrier.arrive_and_wait();
        for(int i = 1; i <= 200000; i++)
        {
            queue.push(Job { i, i });
            std::this_thread::yield();
        }
    });

    // 3 partial sums
    long long sum1 = 0;
    long long sum2 = 0;

    auto consumer_fn = [&queue, &barrier](long long& sum) {
        barrier.arrive_and_wait();
        for(int i = 0; i < 100000; i++)
        {
            while(true)
            {
                std::optional<Job> job = queue.try_pop();
                if(job)
                {
                    sum += job->id;
                    break;
                }
            }
        }
    };

    auto consumer1 = std::thread(consumer_fn, std::ref(sum1));
    auto consumer2 = std::thread(consumer_fn, std::ref(sum2));

    producer1.join();
    consumer1.join();
    consumer2.join();

    printf("magic number: %jd\n", static_cast<intmax_t>(sum1 + sum2));
}

