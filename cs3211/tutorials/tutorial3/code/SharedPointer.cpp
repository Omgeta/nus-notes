// reference_counting.cpp

#include <cstdio>
#include <memory>

void rc(const char* s, const std::shared_ptr<int>& x)
{
    auto refs = x.use_count();
    printf("%s: %zu ref%s\n", s, refs, refs == 1 ? "" : "s");
}

void foo(std::shared_ptr<int> arg)  // num refs: 5
{
    rc("foo 1", arg);
    auto z = arg;   // num refs: 6
    rc("foo 2", arg);

} // num refs: 4 (`arg` & `z` are gone)

int main()
{
    auto x = std::make_shared<int>(100);
    rc("initial", x);

    {
        auto a = x;     rc("A", x); // 2
        auto b = x;     rc("B", x); // 3
        auto c = a;     rc("C", x); // 4
        foo(x);         rc("D", x); // 4
    }

    rc("E", x); // 1 (all the above are gone)

} // num refs: 0, destructor called
