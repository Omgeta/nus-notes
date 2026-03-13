// shared_ptr_circular.cpp

#include <memory>


// Doubly Linked List
struct DLLNode
{
	std::shared_ptr<DLLNode> prev;
	std::shared_ptr<DLLNode> next;
};

struct DLL
{
	std::shared_ptr<DLLNode> head {};
	std::shared_ptr<DLLNode> tail {};

	void push_front(std::shared_ptr<DLLNode>);
	void push_back(std::shared_ptr<DLLNode>);

	std::shared_ptr<DLLNode> front();
	std::shared_ptr<DLLNode> back();
};


int main()
{
	auto a = std::make_shared<DLLNode>();
	auto b = std::make_shared<DLLNode>();

	a->next = b;
	b->prev = a;
}
