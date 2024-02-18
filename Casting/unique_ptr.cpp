#include <memory>
class Task
{
public:
  int mId;
  Task(int id ) :mId(id)
  {
 
  }
  Task()
  {
  }
  ~Task()
  {
 
  }
};
class Child: public Task
{
public:
};

std::unique_ptr<Task> fn()
{
  return std::make_unique<Child>();
}

