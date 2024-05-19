
struct X { int *a, *b; };
namespace std {
  template<typename T, typename ...Args>
  constexpr void construct_at(struct X x, Args &&...args) {
    //new (x.a) T((Args&&)args...); 
  }
}
union A {
    int c;
    X x;
};

union A hello();
constexpr bool bad_construct_at_subobject() {

  A d = {1};
  std::construct_at<int>(((union A) {1}).x, 1); 
  return true;
}
static_assert(bad_construct_at_subobject()); 

