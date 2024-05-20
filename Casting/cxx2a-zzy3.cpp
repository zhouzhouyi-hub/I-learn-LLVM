
struct X { int *a, *b; };
namespace std {
  template<typename T, typename ...Args>
  constexpr void construct_at(struct X x, Args &&...args) {
    //new (x.a) T((Args&&)args...); 
  }
}
struct A {
  int c;
  X x;
  ~A(){};
};

bool bad_construct_at_subobject() {


  std::construct_at<int>(((struct A) {1}).x, 1); 
  return true;
}
//static_assert(bad_construct_at_subobject()); 

