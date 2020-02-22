#include <cstdio>
#include <cstdlib>
#include <new>

#include "..\public.h"

class JsiValueImpl : public JsiValue {
public:
  ~JsiValueImpl() { std::puts("JsiValueImpl destructor called"); }

  bool isNumber() const noexcept { return false; }
};

class Zot : public Foo {
  ~Zot() {}
  void Bar() override {
    std::puts("Zot::Bar called");

    throw std::exception("WHAT!?");
  };

  Value evaluateJavaScript(const char *buffer, const char *sourceURL) override {
    Value value;
    value.data_.pointer = new JsiValueImpl();
    return value;
  }

  int hasDefaultImpl() override { return 43; }
};

void *operator new(std::size_t sz) {
  std::printf("global op new called, size = %zu\n", sz);
  void *ptr = std::malloc(sz);
  if (ptr)
    return ptr;
  else
    throw std::bad_alloc{};
}
void operator delete(void *ptr) noexcept {
  std::puts("global op delete called");
  std::free(ptr);
}

Foo *MakeFoo() { return new Zot(); }