#include <iostream>
#include <string>

struct A {
  char x;
  std::string y;  // 7 bytes padding is added before y to satisfy alignment
  std::string saga;

  A() : x('1'), y(), saga("default constructed") {
    std::cout << "Default constructor called" << std::endl;
  }

  A(int i)
      : x('1' + (char)i),
        y("This string was constructed with A(int)"),
        saga("A(int) constructed") {
    std::cout << "A(int) constructor called" << std::endl;
  }

  ~A() { std::cout << "Destructor called, object was " << saga << std::endl; }
};

struct B {
  char x;
  std::string y;  // 7 bytes padding is added before y to satisfy alignment
  std::string saga;

  B() : x('1'), y(), saga("default constructed") {
    std::cout << "Default constructor called" << std::endl;
  }

  B(int i)
      : x('1' + (char)i),
        y("This string was constructed with B(int)"),
        saga("B(int) constructed") {
    std::cout << "B(int) constructor called" << std::endl;
  }

  B(const B& other)
      : x(other.x),
        y(other.y),
        saga("copy constructed from object that was " + other.saga) {
    std::cout << "Copy constructor called" << std::endl;
  }

  B& operator=(const B& other) {
    std::cout << "Copy assignment called" << std::endl;
    x = other.x;
    y = other.y;
    saga = "(" + saga + ") then set to object that was (" + other.saga + ")";
    return *this;
  }

  ~B() { std::cout << "Destructor called, object was " << saga << std::endl; }
};

struct C {
  char x;
  std::string y;  // 7 bytes padding is added before y to satisfy alignment
  std::string saga;

  C() : x('1'), y(), saga("default constructed") {
    std::cout << "Default constructor called" << std::endl;
  }

  C(int i)
      : x('1' + (char)i),
        y("This string was constructed with C(int)"),
        saga("C(int) constructed") {
    std::cout << "C(int) constructor called" << std::endl;
  }

  C(const C& other)
      : x(other.x),
        y(other.y),
        saga("copy constructed from object that was " + other.saga) {
    std::cout << "Copy constructor called" << std::endl;
  }

  C& operator=(const C& other) {
    std::cout << "Copy assignment called" << std::endl;
    x = other.x;
    y = other.y;
    saga = "(" + saga + ") then set to object that was (" + other.saga + ")";
    return *this;
  }

  C(C&& other)
      : x(std::move(other.x)),
        y(std::move(other.y)),
        saga("move constructed from object that was " + other.saga) {
    std::cout << "Move constructor called" << std::endl;
  }

  C& operator=(C&& other) {
    std::cout << "Move assignment called" << std::endl;
    x = std::move(other.x);
    y = std::move(other.y);
    saga = "(" + saga + ") then set to object that was (" + other.saga + ")";
    other.saga = "(" + other.saga + ") then moved from";
    return *this;
  }

  ~C() { std::cout << "Destructor called, object was " << saga << std::endl; }
};

void demo1() {
  A a;
  { A b; }
  A c;
}

void change_x_bad(A a) { a.x = 'C'; }

void change_x_good(A& a) { a.x = 'C'; }

void demo2() {
  A a;
  change_x_bad(a);
  std::cout << "Mutate pass by value: " << a.x << std::endl;
    
  {
    A a_copy = a;
    a_copy.x = 'C';
    std::cout << "Mutate copy: " << a.x << std::endl;
    a = a_copy;
    std::cout << "Copy mutated back to a: " << a.x << std::endl;
  }

  A b;
  change_x_good(b);
  std::cout << "Mutate pass by reference: " << b.x << std::endl;

  A c;
  A& c_ref = c;
  c_ref.x = 'C';
  std::cout << "Mutate reference: " << c.x << std::endl;
}

void demo3() {
  B a;
  B a_copy = a;
  B b(1);
  b = a;
}

void demo4() {
  C c1;
  std::cout << "c1: " << c1.x << ":" << c1.y << std::endl;
  C c2(1);
  std::cout << "c2: " << c2.x << ":" << c2.y << std::endl;
  C c3 = c2;
  std::cout << "c3: " << c3.x << ":" << c3.y << std::endl;
  c3 = c1;
  std::cout << "c3: " << c3.x << ":" << c3.y << std::endl;
  c3 = std::move(c2);
  std::cout << "c2: " << c2.x << ":" << c2.y << std::endl;
  std::cout << "c3: " << c3.x << ":" << c3.y << std::endl;
}

int main() {
  std::cout << "Demo 1:" << std::endl;
  demo1();
  std::cout << "----" << std::endl;

  std::cout << "Demo 2:" << std::endl;
  demo2();
  std::cout << "----" << std::endl;

  std::cout << "Demo 3:" << std::endl;
  demo3();
  std::cout << "----" << std::endl;

  std::cout << "Demo 4:" << std::endl;
  demo4();
  std::cout << "----" << std::endl;

  return 0;
}
