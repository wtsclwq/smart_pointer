#include <type_traits>
template <typename T>
class smart_ptr {
 public:
  explicit smart_ptr(T *ptr = nullptr) : ptr_(ptr) {}

  // 带模板的构造函数不会被编译器识别为移动构造函数，因此需要显式delete拷贝构造函数和移动构造函数
  smart_ptr(const smart_ptr &) = delete;
  smart_ptr(smart_ptr &&) = delete;
  auto operator=(smart_ptr &&) -> smart_ptr & = delete;

  /*
   * 为了确保子类指针能够移动给基类指针
   * smart_ptr<circle> pc{new circle()};
   * smart_ptr<shape> ps = std::move(pc)
   * 假设T是shape,U是circle
   * 在调用other.release()的过程中，新建一个T*类型的指针ptr，
   * 并将U*类型的other.ptr_赋值给它，如果是U是子类，T是基类，
   * 这种赋值编译器是允许的，但如果不是，就会在编译阶段报错
   */
  template <typename U>
  explicit smart_ptr(smart_ptr<U> &&other) : ptr_(other.release()) {}

  auto operator=(const smart_ptr other) -> smart_ptr & {
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