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
  
int main()
{
    const std::unique_ptr<Task> taskPtr(new Task(23));
    bool b = isa_impl_cl<Task1, std::unique_ptr<Task> const>::doit(taskPtr); 
}
}
