void fun(int* ptr, int x) 
{
    if (ptr == 0)
        throw ptr;
    if (x == 0)
        throw x;
    return;
}

void fun1(int* ptr, int x)
{
  fun(ptr, x);
}
 
int main()
{
  int ret = 0;
  try {
    fun1(0, 0);
  }
  catch (int i) {
    ret = 0x77;
  }
  catch (int *) {
    ret = 0x88; 
  }
  return ret;
}
