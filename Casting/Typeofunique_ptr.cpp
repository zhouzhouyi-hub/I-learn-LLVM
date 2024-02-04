#include <memory>
#include <type_traits>
struct ZZY {
  int i;
};
int main()
{
  const std::unique_ptr<ZZY> ptr(new ZZY);
  using PtrType = typename decltype(ptr)::element_type;
  bool areTypesEqual = std::is_same<ZZY, PtrType>::value;
  PtrType myValue = *ptr; 
  return 0;
}
