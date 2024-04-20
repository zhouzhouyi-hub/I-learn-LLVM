//clang++ -v -S -stdlib=libc++ -std=c++2a  -DNEW=__builtin_operator_new -DDELETE=__builtin_operator_delete ./cxx2a-constexpr-dynalloc.cpp
// When Clang compile this program, findSubobject's Sub.Entries.size()>0
// reduced from Clang's clang/test/SemaCXX/cxx2a-constexpr-dynalloc.cpp
namespace std {
  using size_t = decltype(sizeof(0));
  template<typename T> struct allocator {
    constexpr T *allocate(size_t N) {
      return (T*)NEW(sizeof(T) * N); 
    }
    constexpr void deallocate(void *p) {
      DELETE(p); 
    }
  };
}

constexpr int no_lifetime_start = (*std::allocator<int>().allocate(1) = 1); 
