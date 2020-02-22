#pragma once
#include <string>

#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

struct JsiValue {
  virtual ~JsiValue() = default;

  virtual bool isNumber() const noexcept = 0;
};

class Value {
public:
  enum ValueKind {
    UndefinedKind,
    NullKind,
    BooleanKind,
    NumberKind,
    SymbolKind,
    StringKind,
    ObjectKind,
    PointerKind = SymbolKind,
  };

  union Data {
    // Value's ctor and dtor will manage the lifecycle of the contained Data.
    Data() {
      static_assert(sizeof(Data) == sizeof(uint64_t),
                    "Value data should fit in a 64-bit register");
    }
    ~Data() {}

    // scalars
    bool boolean;
    double number;
    // pointers
    JsiValue *pointer; // Symbol, String, Object, Array, Function
  };

  ValueKind kind_;
  Data data_;
};

namespace abi {

template <class T> struct basic_string_view {
  virtual ~basic_string_view() = default;

  virtual const T *data() const noexcept = 0;
  virtual size_t size() const noexcept = 0;

  virtual operator std::string() = 0;
};

using string_view = basic_string_view<char>;

struct string_view_impl : public basic_string_view<char> {
  string_view_impl(std::string &&str) : _str(str) {}

  operator std::string() override { return _str; }

  const char* data() const noexcept override { return _str.c_str(); }
  size_t size() const noexcept override { return _str.size(); }

private:
  std::string _str;
};

} // namespace abi

struct Foo {
protected:
  Foo() = default;

public:
  virtual ~Foo() = default;
  virtual void Bar() = 0;

  virtual Value evaluateJavaScript(const char *buffer,
                                   const char *sourceURL) = 0;

  virtual int hasDefaultImpl() { return 42; }
};

extern "C" DLL_EXPORT Foo *MakeFoo();
