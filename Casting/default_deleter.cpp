#include <iostream>
#include <memory>

class MyClass {
public:
    void display() const {
        std::cout << "MyClass object\n";
    }
};

class MyChild: public MyClass {
};
int main() {
    // Create the first std::default_delete object
    std::default_delete<MyChild> deleter1;

    // Construct another std::default_delete object from the first one
    std::default_delete<MyClass> deleter2(deleter1);

    // Use deleter2 to delete a dynamically allocated object
    MyClass* ptr = new MyChild();
    deleter2(ptr);  // deletes the object

    return 0;
}
