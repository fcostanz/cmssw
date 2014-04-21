#ifndef Tracker_ChurnAllocator_H
#define Tracker_ChurnAllocator_H
#include <memory>

template <typename T>
class churn_allocator: public std::allocator<T>
{
public:
  using Base = std::allocator<T>;
  using pointer = typename Base::pointer;
  using size_type = typename Base::size_type;

  static pointer&  cache() {
    static thread_local pointer local = nullptr;
    return local;
  } 
  static bool & gard() {
    static thread_local bool g = false;
    return g;
  }


  template<typename _Tp1>
  struct rebind
  {
    typedef churn_allocator<_Tp1> other;
  };

  pointer allocate(size_type n, const void *hint=0)
  {
    if (!gard()) 
     cache() = std::allocator<T>::allocate(n, hint);
    gard()=false; return cache();
  }
  
  void deallocate(pointer p, size_type n)
  {
    if (p==cache()) gard()=true;
    else std::allocator<T>::deallocate(p, n);
  }
  
  churn_allocator() = default;
  churn_allocator(churn_allocator const&)=default;
  churn_allocator(churn_allocator &&)=default;

  template <class U>                    
  churn_allocator(const churn_allocator<U> &a) noexcept: std::allocator<T>(a) { }
};

#endif
