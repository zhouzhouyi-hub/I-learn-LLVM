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

template <typename To, typename From>
struct isa_impl_cl<To,  std::unique_ptr<From>> {
  static inline bool doit( std::unique_ptr<From> &Val) {
    return true;
    //     return isa_impl_cl<To, From>::doit(*Val);
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

struct Task
{
  int mId;
  Task(int id ) :mId(id)
  {
  }
  ~Task()
  {

  }
  static bool classof(const Task *D)
  {return true;}
};

struct Task1
{
  int mId;
  Task1(int id ) :mId(id)
  {

  }
  ~Task1()
  {

  }
  static bool classof(const Task * D)
  {return true;}
};  

template <typename To, typename From, typename Enable = void>
struct CastIsPossible {
  static inline bool isPossible(const From &f) {
    return isa_impl_wrap<
      To, const From,
      typename simplify_type<const From>::SimpleType>::doit(f);
  }
};
#if 0
template <typename To, typename From, typename Enable = void>
struct CastInfo : public CastIsPossible<To, From> {
  using Self = CastInfo<To, From, Enable>;
  
  using CastReturnType = typename cast_retty<To, From>::ret_type;
  
  static inline CastReturnType doCast(const From &f) {
    return cast_convert_val<
      To, From,
      typename simplify_type<From>::SimpleType>::doit(const_cast<From &>(f));
  }
  
  // This assumes that you can construct the cast return type from `nullptr`.
  // This is largely to support legacy use cases - if you don't want this
  // behavior you should specialize CastInfo for your use case.
  static inline CastReturnType castFailed() { return CastReturnType(nullptr); }
  
  static inline CastReturnType doCastIfPossible(const From &f) {
    if (!Self::isPossible(f))
      return castFailed();
    return doCast(f);
  }
};
#endif  
}

int main()
{
  const std::unique_ptr<llvm::Task> taskPtr(new llvm::Task(23));
  bool b = llvm::isa_impl_cl<llvm::Task1, std::unique_ptr<llvm::Task> const>::doit(taskPtr);
  return 0;
}
