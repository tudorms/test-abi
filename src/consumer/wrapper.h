#include "..\public.h"

#include <memory>
#include <string>

namespace Wrapped {

struct Foo {
protected:
  std::unique_ptr<::Foo> _foo;

public:
  Foo(::Foo *foo) : _foo(foo) {}
  void Bar() { _foo->Bar(); }

  inline std::shared_ptr<JsiValue>
  evaluateJavaScript(const std::string &buffer, const std::string &sourceURL) {
    return std::shared_ptr<JsiValue>(
        _foo->evaluateJavaScript(buffer.c_str(), sourceURL.c_str()));
  }
};

std::shared_ptr<Foo> MakeFoo() { return std::make_shared<Foo>(::MakeFoo()); }
} // namespace Wrapped
