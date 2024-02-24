#include <memory>
class Sibling;
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
  Task(class Sibling &&sib) {}
};
class Sibling
{
public:
  
};

std::unique_ptr<Task> fn()
{
  return std::make_unique<Sibling>();
}

int main()
{
  fn();
  return 0;
}
