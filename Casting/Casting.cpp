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

template <class To, class From> struct cast_retty;

template <class To, class From> struct cast_retty_impl {
  using ret_type = To &; // Normal case, return Ty&
};
template <class To, class From> struct cast_retty_impl<To, const From> {
  using ret_type = const To &; // Normal case, return Ty&
};

template <class To, class From> struct cast_retty_impl<To, From *> {
  using ret_type = To *; // Pointer arg case, return Ty*
};

template <class To, class From> struct cast_retty_impl<To, const From *> {
  using ret_type = const To *; // Constant pointer arg case, return const Ty*
};

template <class To, class From> struct cast_retty_impl<To, const From *const> {
  using ret_type = const To *; // Constant pointer arg case, return const Ty*
};

template <class To, class From>
struct cast_retty_impl<To, std::unique_ptr<From>> {
private:
  using PointerType = typename cast_retty_impl<To, From *>::ret_type;
  using ResultType = std::remove_pointer_t<PointerType>;

public:
  using ret_type = std::unique_ptr<ResultType>;
};

template <class To, class From, class SimpleFrom> struct cast_retty_wrap {
  using ret_type = typename cast_retty<To, SimpleFrom>::ret_type;
};
 
template <class To, class FromTy> struct cast_retty_wrap<To, FromTy, FromTy> {
  using ret_type = typename cast_retty_impl<To, FromTy>::ret_type;
};
 
template <class To, class From> struct cast_retty {
  using ret_type = typename cast_retty_wrap<
      To, From, typename simplify_type<From>::SimpleType>::ret_type;
};
  
class Task
{
public:
  int mId;
    
  Task(){}
  Task(int id ) :mId(id)
  {
  }
  ~Task()
  {

  }
  static bool classof(const Task *D)
  {return true;}
};

class Task1:public Task
{
public:
  
  Task1() 
  {
  }
  ~Task1()
  {

  }
};  


template <typename To, typename From, typename Enable = void>
struct CastIsPossible {
  static inline bool isPossible(const From &f) {
    return isa_impl_wrap<
      To, const From,
      typename simplify_type<const From>::SimpleType>::doit(f);
  }
};

template <class To, class From, class SimpleFrom> struct cast_convert_val {
  static typename cast_retty<To, From>::ret_type doit(const From &Val) {
    return cast_convert_val<To, SimpleFrom,
                            typename simplify_type<SimpleFrom>::SimpleType>::
        doit(simplify_type<From>::getSimplifiedValue(const_cast<From &>(Val)));
  }
};

template <class To, class FromTy> struct cast_convert_val<To, FromTy, FromTy> {
  static typename cast_retty<To, FromTy>::ret_type doit(const FromTy &Val) {
    return *(std::remove_reference_t<typename cast_retty<To, FromTy>::ret_type>
                 *)&const_cast<FromTy &>(Val);
  }
};

template <class To, class FromTy>
struct cast_convert_val<To, FromTy *, FromTy *> {
  static typename cast_retty<To, FromTy *>::ret_type doit(const FromTy *Val) {
    return (typename cast_retty<To, FromTy *>::ret_type) const_cast<FromTy *>(
        Val);
  }
};
template <typename To, typename From, typename Derived = void>
struct UniquePtrCast : public CastIsPossible<To, From *> {
// using Self = detail::SelfType<Derived, UniquePtrCast<To, From>>;
  using CastResultType = std::unique_ptr<
    std::remove_reference_t<typename cast_retty<To, From>::ret_type>>;
 
  static inline CastResultType doCast(std::unique_ptr<From> &&f) {
    return CastResultType((typename CastResultType::element_type *)f.release());
  }
 
  static inline CastResultType castFailed() { return CastResultType(nullptr); }
  
  /*  static inline CastResultType doCastIfPossible(std::unique_ptr<From> &f) {
    if (!Self::isPossible(f.get()))
      return castFailed();
    return doCast(std::move(f));
  }*/
};

template <typename To, typename From, typename Enable = void>
struct CastInfo : public CastIsPossible<To, From> {

  using Self = CastInfo<To, From, Enable>;
  
  using CastReturnType = typename cast_retty<To, From>::ret_type;

  static inline CastReturnType doCast(const From &f) {
    return cast_convert_val<
      To, From,
      typename simplify_type<From>::SimpleType>::doit(const_cast<From &>(f));
  }
  
  static inline CastReturnType castFailed() { return CastReturnType(nullptr); }

  static inline CastReturnType doCastIfPossible(const From &f) {
    if (!Self::isPossible(f))
      return castFailed();
    return doCast(f);
  }
};

template <typename To, typename From>
struct CastInfo<To, std::unique_ptr<From>> : public UniquePtrCast<To, From> {};

template <typename To, typename From>
bool isa(const From &Val) {
   return CastInfo<To, const From>::isPossible(Val);
}
  
template <typename To, typename From>
 decltype(auto) cast(std::unique_ptr<From> &&Val) {
  bool b = isa<To>(Val);
  return CastInfo<To, std::unique_ptr<From>>::doCast(std::move(Val));
}
  
}


int main()
{
  // const std::unique_ptr<llvm::Task> taskPtr(new llvm::Task(/*23*/));
  //const std::unique_ptr<llvm::Task1> taskPtr1(new llvm::Task1);
  //std::unique_ptr<llvm::Task1> taskPtr1(new llvm::Task1);

  //bool b = llvm::isa_impl_cl<llvm::Task1, std::unique_ptr<llvm::Task> const>::doit(taskPtr);
  //llvm::cast<llvm::Task>(std::move(taskPtr1));
  std::unique_ptr<llvm::Task> tk = llvm::cast<llvm::Task>(std::make_unique<llvm::Task1>());
  return 0;
}
