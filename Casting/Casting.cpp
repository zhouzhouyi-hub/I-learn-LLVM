#include <memory>
namespace llvm {
template <typename From> struct simplify_type {
  using SimpleType = From; 
  static SimpleType &getSimplifiedValue(From &Val) { return Val; }
};

template <typename To, typename From, typename Enabler = void> struct isa_impl {
  static inline bool doit(const From &Val) { return To::classof(&Val); }
};

template <typename To, typename From> struct isa_impl_cl {
  static inline bool doit(const From &Val) {
    return isa_impl<To, From>::doit(Val);
  }
};

template <typename To, typename From> struct isa_impl_cl<To, const From> {
  static inline bool doit(const From &Val) {
    return isa_impl<To, From>::doit(Val);
  }
};

template <typename To, typename From>
struct isa_impl_cl<To, const std::unique_ptr<From>> {
  static inline bool doit(const std::unique_ptr<From> &Val) {
    return isa_impl_cl<To, From>::doit(*Val);
  }
};

template <typename To, typename From, typename SimpleFrom>
struct isa_impl_wrap {
  // When From != SimplifiedType, we can simplify the type some more by using
  // the simplify_type template.
  static bool doit(const From &Val) {
    return isa_impl_wrap<To, SimpleFrom,
                         typename simplify_type<SimpleFrom>::SimpleType>::
        doit(simplify_type<const From>::getSimplifiedValue(Val));
  }
};
 
template <typename To, typename FromTy>
struct isa_impl_wrap<To, FromTy, FromTy> {

  static bool doit(const FromTy &Val) {
    return isa_impl_cl<To, FromTy>::doit(Val);
  }
};

}
