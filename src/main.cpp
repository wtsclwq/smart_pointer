/*
 * @Description:
 * @author: wtsclwq
 * @Date: 2023-03-05 22:38:06
 * @LastEditTime: 2023-03-13 00:17:57
 */
#include <cstdio>

#include "shared_ptr.hpp"
class shape {
 public:
  shape() = default;
  shape(const shape &) = default;
  shape(shape &&) = delete;
  auto operator=(const shape &) -> shape & = default;
  auto operator=(shape &&) -> shape & = delete;
  virtual ~shape() = default;
};

class circle : public shape {
 public:
  circle() = default;
  circle(const circle &) = default;
  circle(circle &&) = delete;
  auto operator=(const circle &) -> circle & = default;
  auto operator=(circle &&) -> circle & = delete;
  ~circle() override { puts("~circle()"); }
};

auto main() -> int {
  shared_ptr<circle> ptr1(new circle());
  printf("use count of ptr1 is %ld\n", ptr1.use_count());
  shared_ptr<shape> ptr2;
  printf("use count of ptr2 was %ld\n", ptr2.use_count());
  ptr2 = ptr1;
  printf("use count of ptr2 is now %ld\n", ptr2.use_count());
  if (ptr1) {
    puts("ptr1 is not empty");
  }
  shared_ptr<circle> ptr3 = dynamic_pointer_cast<circle>(ptr2);
  printf("use count of ptr3 is %ld\n", ptr3.use_count());
}