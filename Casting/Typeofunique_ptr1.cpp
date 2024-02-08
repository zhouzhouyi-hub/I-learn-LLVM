#include <memory>
#include <type_traits>
class Task
{
public:
  int mId;
  Task(){}
  Task(int id ) :mId(id)
  {
  }
  ~Task()
  {

  }
  static bool classof(const Task *D)
  {return true;}
};

class Task1:public Task
{
public:
  Task1() 
  {
  }
  ~Task1()
  {

  }
};

std::unique_ptr<Task> foo()
{

  return std::make_unique<Task1>();

}
int main()
{
  const std::unique_ptr<Task> ptr = foo();
  return 0;
}
