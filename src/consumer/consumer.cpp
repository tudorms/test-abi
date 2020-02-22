#include "wrapper.h"

int main() {
  auto f = Wrapped::MakeFoo();
  try {
  f->Bar();
  }
  catch(std::exception& ex) {
    std::puts(ex.what());
  }
  auto result = f->evaluateJavaScript("test", "url");

  std::printf("result->isNumber = %s\n", result.data_.pointer->isNumber() ? "yes" :"no");
  std::printf("exported = %d\n", f->hasDefaultImpl());
}