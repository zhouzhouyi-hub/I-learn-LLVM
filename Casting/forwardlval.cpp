int && forward (int &i) {return static_cast<int &&>(i);}
int && forward (int &&i) {return static_cast<int &&>(i);}

void fn(int && j){
  forward(j);
}
int main()
{
  fn(1);
  return 0;
}
