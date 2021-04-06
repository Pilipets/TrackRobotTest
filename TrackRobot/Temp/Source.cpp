#include <memory>
#include <iostream>
using namespace std;

struct MyClass : std::enable_shared_from_this<MyClass> {
    shared_ptr<MyClass> method() {
        return shared_from_this();
    }
};

int main() {
    auto ptr = make_shared<MyClass>();
    auto ptr2 = ptr->method();

    cout << ptr.use_count() << "\n";
}