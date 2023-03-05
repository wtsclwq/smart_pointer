#include <type_traits>
template <typename T>
class smart_ptr {
 public:
  explicit smart_ptr(T *ptr = nullptr) : ptr_(ptr) {}
  // smart_ptr ptr1{create_shape(shape_type::circle)};
  // smart_ptr ptr2{ptr1};
  // 使用拷贝构造函数的时候，新对象ptr2直接将ptr1的指针所有权拿走
  smart_ptr(const smart_ptr &other) : ptr_(other.release()) {}

  // smart_ptr ptr1{create_shape(shape_type::circle)};
  // smart_ptr ptr2 = ptr1;
  // 首先使用拷贝构造函数，传入other(ptr1)创建一个新的对象，此时other(ptr1)的指针所有权已经转移给了这个临时对象temp，即失去了所有权
  // 然后调用这个临时对象的swap方法，将临时对象temp.ptr和当前对象(ptr2)this->ptr_互换，那么当前对象ptr2就拿到了other(ptr1)的指针所有权
  auto operator=(const smart_ptr &other) -> smart_ptr & {
    smart_ptr(other).swap(*this);
    return *this;
  };

  smart_ptr(smart_ptr &&) = delete;
  auto operator=(smart_ptr &&) -> smart_ptr & = delete;

  auto operator*() const -> T & { return *ptr_; }
  auto operator->() const -> T * { return ptr_; }
  explicit operator bool() const { return ptr_; }
  ~smart_ptr() { delete ptr_; }

  // 交出指针所有权
  auto release() -> T * {
    T *ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

  void swap(smart_ptr &rhs) { std::swap(ptr_, rhs.ptr); }
  auto get() const -> T * { return ptr_; }

 private:
  T *ptr_;
};