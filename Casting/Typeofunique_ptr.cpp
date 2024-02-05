#include <memory>
#include <type_traits>
struct Task
{
  int mId;
  Task(int id ) :mId(id)
  {
  }
  ~Task()
  {

  }
  static bool classof(const Task *D)
  {return true;}
};

struct Task1
{
  int mId;
  Task1(int id ) :mId(id)
  {

  }
  ~Task1()
  {

  }
  static bool classof(const Task * D)
  {return true;}
};  
int main()
{
  const std::unique_ptr<Task1> ptr(new Task1(3));
  Task D(3);
  using PtrType = typename decltype(ptr)::element_type;
  //bool areTypesEqual = std::is_same<ZZY, PtrType>::value;
  bool artTypesEqual = PtrType::classof(&D);
  //PtrType myValue = *ptr; 
  return 0;
}
