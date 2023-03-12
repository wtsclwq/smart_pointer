
#include <cstdint>
#include <utility>  // std::swap

class shared_count {
 public:
  shared_count() noexcept : count_(1) {}
  void add_count() noexcept { ++count_; }
  long reduce_count() noexcept { return --count_; }
  long get_count() const noexcept { return count_; }

 private:
  long count_;
};

template <typename T>
class shared_ptr {
 public:
  template <typename U>
  friend class shared_ptr;

  explicit shared_ptr(T* ptr = nullptr) : ptr_(ptr) {
    if (ptr) {
      shared_count_ = new shared_count();
    }
  }
  ~shared_ptr() {
    if (ptr_ && (shared_count_->reduce_count() == 0)) {
      delete ptr_;
      delete shared_count_;
    }
  }

  shared_ptr(const shared_ptr& other) : ptr_(other.ptr_) {
    
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }
  template <typename U>
  shared_ptr(const shared_ptr<U>& other) noexcept : ptr_(other.ptr_) {
    
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }
  template <typename U>
  explicit shared_ptr(shared_ptr<U>&& other) noexcept : ptr_(other.ptr_) {
    
    if (ptr_) {
      shared_count_ = other.shared_count_;
      other.ptr_ = nullptr;
    }
  }
  template <typename U>
  shared_ptr(const shared_ptr<U>& other, T* ptr) noexcept : ptr_(ptr) {
    
    if (ptr_) {
      other.shared_count_->add_count();
      shared_count_ = other.shared_count_;
    }
  }
  auto operator=(shared_ptr rhs) noexcept -> shared_ptr& {
    rhs.swap(*this);
    return *this;
  }

  auto get() const noexcept -> T* { return ptr_; }
  auto use_count() const noexcept -> uint64_t {
    if (ptr_) {
      return shared_count_->get_count();
    }
    return 0;
  }
  void swap(shared_ptr& rhs) noexcept {
    using std::swap;
    swap(ptr_, rhs.ptr_);
    swap(shared_count_, rhs.shared_count_);
  }

  auto operator*() const noexcept -> T& { return *ptr_; }
  auto operator->() const noexcept -> T* { return ptr_; }
  explicit operator bool() const noexcept { return ptr_; }

 private:
  T* ptr_;
  shared_count* shared_count_;
};

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept {
  lhs.swap(rhs);
}

template <typename T, typename U>
auto static_pointer_cast(const shared_ptr<U>& other) noexcept -> shared_ptr<T> {
  T* ptr = static_cast<T*>(other.get());
  return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
auto reinterpret_pointer_cast(const shared_ptr<U>& other) noexcept
    -> shared_ptr<T> {
  T* ptr = reinterpret_cast<T*>(other.get()); // NOLINT
  return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
auto const_pointer_cast(const shared_ptr<U>& other) noexcept -> shared_ptr<T> {
  T* ptr = const_cast<T*>(other.get());  // NOLINT
  return shared_ptr<T>(other, ptr);
}

template <typename T, typename U>
auto dynamic_pointer_cast(const shared_ptr<U>& other) noexcept
    -> shared_ptr<T> {
  T* ptr = dynamic_cast<T*>(other.get());
  return shared_ptr<T>(other, ptr);
}