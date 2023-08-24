#include <type_traits>
#include <iostream>

using namespace std;

class Base {};

class Derived : public Base {};

template <typename T, class = std::enable_if_t<std::is_base_of_v<Base, std::decay_t<T>>>>
void foo (int x) {
    cout << "true" << endl;
}

// template <typename T>
// void foo(float x) {
//     cout << "false" << endl;
// }

int main() {
    Derived d;
    Derived &rd = d;

    cout << "regular: ";
    foo<decltype(d)>(5);

    cout << "ref: ";
    foo<decltype(rd)>(5);

    return 0;
}
