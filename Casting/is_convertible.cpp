#include "__type_traits/is_convertible.h"
class foo{
};
class bar{
};
int main()
{
  bool AtoB = std::is_convertible<foo*, foo*>::value;
}

