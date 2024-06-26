// RUN: %clang_cc1 -std=c++2a -verify %s -DNEW=__builtin_operator_new -DDELETE=__builtin_operator_delete
// RUN: %clang_cc1 -std=c++2a -verify %s "-DNEW=operator new" "-DDELETE=operator delete"
// RUN: %clang_cc1 -std=c++2a -verify %s "-DNEW=::operator new" "-DDELETE=::operator delete"

constexpr bool alloc_from_user_code() {
  void *p = NEW(sizeof(int)); // expected-note {{cannot allocate untyped memory in a constant expression; use 'std::allocator<T>::allocate'}}
  DELETE(p);
  return true;
}
static_assert(alloc_from_user_code()); // expected-error {{constant expression}} expected-note {{in call}}

namespace std {
  using size_t = decltype(sizeof(0));
  // FIXME: It would be preferable to point these notes at the location of the call to allocator<...>::[de]allocate instead
  template<typename T> struct allocator {
    constexpr T *allocate(size_t N) {
      return (T*)NEW(sizeof(T) * N); // expected-note 3{{heap allocation}} expected-note {{not deallocated}}
    }
    constexpr void deallocate(void *p) {
      DELETE(p); // #dealloc expected-note 2{{'std::allocator<...>::deallocate' used to delete pointer to object allocated with 'new'}}
    }
  };
}

constexpr bool alloc_via_std_allocator() {
  std::allocator<int> alloc;
  int *p = alloc.allocate(1);
  alloc.deallocate(p);
  return true;
}
static_assert(alloc_via_std_allocator());

template<> struct std::allocator<void()> {
  constexpr void *allocate() { return NEW(8); } // expected-note {{cannot allocate memory of function type 'void ()'}}
};
constexpr void *fn = std::allocator<void()>().allocate(); // expected-error {{constant expression}} expected-note {{in call}}

struct Incomplete;
template<> struct std::allocator<Incomplete> {
  constexpr void *allocate() { return NEW(8); } // expected-note {{cannot allocate memory of incomplete type 'Incomplete'}}
};
constexpr void *incomplete = std::allocator<Incomplete>().allocate(); // expected-error {{constant expression}} expected-note {{in call}}

struct WrongSize { char x[5]; };
static_assert(sizeof(WrongSize) == 5);
template<> struct std::allocator<WrongSize> {
  constexpr void *allocate() { return NEW(7); } // expected-note {{allocated size 7 is not a multiple of size 5 of element type 'WrongSize'}}
};
constexpr void *wrong_size = std::allocator<WrongSize>().allocate(); // expected-error {{constant expression}} expected-note {{in call}}

constexpr bool mismatched(int alloc_kind, int dealloc_kind) {
  int *p;
  switch (alloc_kind) {
  case 0:
    p = new int; // expected-note {{heap allocation}}
    break;
  case 1:
    p = new int[1]; // expected-note {{heap allocation}}
    break;
  case 2:
    p = std::allocator<int>().allocate(1);
    break;
  }
  switch (dealloc_kind) {
  case 0:
    delete p; // expected-note {{'delete' used to delete pointer to object allocated with 'std::allocator<...>::allocate'}}
    break;
  case 1:
    delete[] p; // expected-note {{'delete' used to delete pointer to object allocated with 'std::allocator<...>::allocate'}}
    break;
  case 2:
    std::allocator<int>().deallocate(p); // expected-note 2{{in call}}
    break;
  }
  return true;
}
static_assert(mismatched(0, 2)); // expected-error {{constant expression}} expected-note {{in call}}
static_assert(mismatched(1, 2)); // expected-error {{constant expression}} expected-note {{in call}}
static_assert(mismatched(2, 0)); // expected-error {{constant expression}} expected-note {{in call}}
static_assert(mismatched(2, 1)); // expected-error {{constant expression}} expected-note {{in call}}
static_assert(mismatched(2, 2));

constexpr int *escape = std::allocator<int>().allocate(3); // expected-error {{constant expression}} expected-note {{pointer to subobject of heap-allocated}}
constexpr int leak = (std::allocator<int>().allocate(3), 0); // expected-error {{constant expression}}
constexpr int no_lifetime_start = (*std::allocator<int>().allocate(1) = 1); // expected-error {{constant expression}} expected-note {{assignment to object outside its lifetime}}
constexpr int no_deallocate_nullptr = (std::allocator<int>().deallocate(nullptr), 1); // expected-error {{constant expression}} expected-note {{in call}}
// expected-note@#dealloc {{'std::allocator<...>::deallocate' used to delete a null pointer}}
constexpr int no_deallocate_nonalloc = (std::allocator<int>().deallocate((int*)&no_deallocate_nonalloc), 1); // expected-error {{constant expression}} expected-note {{in call}}
// expected-note@#dealloc {{delete of pointer '&no_deallocate_nonalloc' that does not point to a heap-allocated object}}
// expected-note@-2 {{declared here}}

void *operator new(std::size_t, void *p) { return p; }
constexpr bool no_placement_new_in_user_code() { // expected-error {{never produces a constant expression}}
  int a;
  new (&a) int(42); // expected-note {{call to placement 'operator new'}}
  return a == 42;
}

namespace std {
  constexpr bool placement_new_in_stdlib() {
    int a;
    new (&a) int(42);
    return a == 42;
  }
}
static_assert(std::placement_new_in_stdlib());

namespace std {
  template<typename T, typename ...Args>
  constexpr void construct_at(void *p, Args &&...args) {
    new (p) T((Args&&)args...); // #new
  }
}

constexpr bool bad_construct_at_type() {
  int a;
  // expected-note@#new {{placement new would change type of storage from 'int' to 'float'}}
  std::construct_at<float>(&a, 1.0f); // expected-note {{in call}}
  return true;
}
static_assert(bad_construct_at_type()); // expected-error{{}} expected-note {{in call}}

constexpr bool bad_construct_at_subobject() {
  struct X { int a, b; };
  union A {
    int c;
    X x;
  };
  A d = {.x = {1, 2}};
  // expected-note@#new {{construction of subobject of member 'x' of union with active member 'a' is not allowed in a constant expression}}
  std::construct_at<int>(&d.x.a, 1); // expected-note {{in call}}
  return true;
}
static_assert(bad_construct_at_subobject()); // expected-error{{}} expected-note {{in call}}

