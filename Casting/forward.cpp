#include <utility>
void foo(int && i)
{
}
void foo(int &i )
{
}
template<class T>
void wrapper(T&& arg)
{
    foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
}

int main()
{
  int i;
  wrapper(1);
  wrapper(i);
}
