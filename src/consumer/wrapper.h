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
  int hasDefaultImpl() { return _foo->hasDefaultImpl(); }

  inline Value
  evaluateJavaScript(const std::string &buffer, const std::string &sourceURL) {
    return _foo->evaluateJavaScript(buffer.c_str(), sourceURL.c_str());
  }
};

std::shared_ptr<Foo> MakeFoo() { return std::make_shared<Foo>(::MakeFoo()); }
} // namespace Wrapped
