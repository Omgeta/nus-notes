#include <bits/stdc++.h>
#include <stdio.h>

void use_foo(int*)
{
}
void use_bar(FILE*)
{
}

int main()
{
    // int* foo = new int { 0 };
    std::unique_ptr<int> foo = std::make_unique<int>(0);

    // FILE* bar = fopen("file.txt", "w");
    auto deleter = [](FILE* f) {
        fclose(f);
    };

    auto bar = std::unique_ptr<FILE, decltype(deleter)> {
        fopen("file.txt", "w"), deleter
    };

    use_foo(foo.get());
    use_bar(bar.get());
} // bar and foo deleted here
