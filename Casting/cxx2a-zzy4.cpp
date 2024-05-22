
struct X { int *a, *b; };
struct A {
  int c;
  X x;
  //~A(){};
};

namespace std {
  template<typename T, typename ...Args>
  constexpr void construct_at(struct A a, Args &&...args) {
    //new (x.a) T((Args&&)args...); 
  }
}

constexpr bool bad_construct_at_subobject() {


  std::construct_at<int>(((struct A) {1}), 1); 
  return true;
}
static_assert(bad_construct_at_subobject()); 

