#include <type_traits>
template <typename T>
class smart_ptr {
 public:
  explicit smart_ptr(T *ptr = nullptr) : ptr_(ptr) {}
  // smart_ptr ptr1{create_shape(shape_type::circle)};
  // smart_ptr ptr2{std::move(ptr1)};
  // 使用移动构造函数的时候，新对象ptr2直接将ptr1的指针所有权拿走
  smart_ptr(const smart_ptr &&other) noexcept : ptr_(other.release()) {}

  // smart_ptr ptr1{create_shape(shape_type::circle)};
  // 参数变成了对象而非引用，那么创建other时必定要隐式调用某个构造函数
  //! 如果直接调用赋值运算符，ptr3=ptr1,此时在构造other时编译器会把ptr3作为参数，隐式调用拷贝构造函数，然而我们没有拷贝构造函数(声明了移动构造但是没有显示声明拷贝构造，拷贝构造自动被禁用)
  //! 如果等号右侧是一个右值引用ptr3 = std::move(ptr1)，这时在创建other时就会自动隐式调用移动构造函数
  // 如果我们同时拥有拷贝(并没有，只有移动，也就是等号右侧只能是右值引用)和移动构造函数，那么赋值运算符间接使用拷贝还是移动就完全可控了
  auto operator=(const smart_ptr other) -> smart_ptr & {
    // 这里的other对象相当于是外界ptr1，因为在构造other时，ptr1已经被移动构造函数拿走了指针的所有权，外界已经无法再使用ptr1了
    // 调用other对象的swap方法和当前对象的ptr互换（当前对象是新对象，ptr还是nullptr）
    other.swap(*this);
    return *this;
  };

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