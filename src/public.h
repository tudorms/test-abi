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

struct Foo {
protected:
  Foo() = default;

public:
  virtual ~Foo() = default;
  virtual void Bar() = 0;

  virtual JsiValue *evaluateJavaScript(const char *buffer,
                                       const char *sourceURL) = 0;
};

extern "C" DLL_EXPORT Foo *MakeFoo();
