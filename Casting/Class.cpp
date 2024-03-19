class A
{
private:
  int data;
public:
  A()  {}
  ~A()
  {

  }
  A(int i):data(i)
  {

  }
};
A Play(A a)
{
  return a;
}

int main()
{
  A tempzzy = Play(10);
  return 0;
}

