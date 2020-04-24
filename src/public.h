#pragma once
#include <string>
#include <string_view>
#include <functional>

#ifdef BUILDING_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


namespace abi {

template <class T> struct basic_string_view
{
	virtual ~basic_string_view() = default;

	virtual const T* data() const noexcept
	{
		return nullptr;
	}

	virtual size_t size() const noexcept
	{
		return 0;
	}

	operator std::string() const
	{
		return std::string(this->data(), this->size());
	}
};

using string_view = basic_string_view<char>;

struct string_view_impl : public string_view
{
	string_view_impl(std::string&& str) : _str(str)
	{
	}

	const char* data() const noexcept override
	{
		return _str.c_str();
	}
	size_t size() const noexcept override
	{
		return _str.size();
	}

private:
	std::string _str;
};

struct string_view_ptr
{
	string_view_ptr(abi::string_view* ptr) : _ptr(ptr)
	{
	}

	~string_view_ptr()
	{
		delete _ptr;
	}

	string_view* operator->() const noexcept
	{
		return const_cast<string_view_ptr*>(this)->_ptr;
	}

	operator std::string() const
	{
		return std::string(_ptr->data(), _ptr->size());
	}

	operator std::string_view() const
	{
		return std::string_view(_ptr->data());
	}

	string_view* _ptr;
};

} // namespace abi

struct JsiValue
{
	virtual ~JsiValue() = default;

	virtual bool isNumber() const noexcept = 0;

	virtual void invalidate() = 0;

	virtual abi::string_view_ptr toString() = 0;
};

class Value
{
public:
	enum ValueKind
	{
		UndefinedKind,
		NullKind,
		BooleanKind,
		NumberKind,
		SymbolKind,
		StringKind,
		ObjectKind,
		PointerKind = SymbolKind,
	};

	union Data
	{
		// Value's ctor and dtor will manage the lifecycle of the contained Data.
		Data()
		{
			static_assert(sizeof(Data) == sizeof(uint64_t),
				"Value data should fit in a 64-bit register");
		}
		~Data()
		{
		}

		// scalars
		bool boolean;
		double number;
		// pointers
		JsiValue* pointer; // Symbol, String, Object, Array, Function
	};

	ValueKind kind_;
	Data data_;
};

using HostFunctionType = std::function<size_t(size_t a, size_t b)>;
using CHostFunctionType = size_t(void*, size_t, size_t);

struct Foo
{
protected:
	Foo() = default;

public:
	virtual ~Foo() = default;
	virtual void Bar() = 0;

	virtual Value evaluateJavaScript(const char* buffer,
		const char* sourceURL) = 0;

	virtual size_t receivesCFunction(CHostFunctionType* funcPointer, void* pv) = 0;

	static size_t std_function_caller(void* fn, size_t a, size_t b)
	{
		return (*static_cast<HostFunctionType*>(fn))(a, b);
	}

	size_t receivesFunction(HostFunctionType fn)
	{
		return receivesCFunction(std_function_caller, static_cast<void*>(&fn));
	}

	virtual int hasDefaultImpl()
	{
		return 42;
	}
};

class DLL_EXPORT JSIException : public std::exception
{
protected:
	JSIException()
	{
	};

	JSIException(std::string what) : what_(std::move(what))
	{
	};

public:
	virtual const char* what() const noexcept override
	{
		return what_.c_str();
	}

protected:
	std::string what_;
};

class JSINativeException : public JSIException
{
public:
	JSINativeException(std::string what) : JSIException(std::move(what))
	{
	}
};

class DLL_EXPORT JSError : public JSIException
{
public:
	JSError(std::shared_ptr<Value> value, std::string message, std::string stack) :
		message_(message), stack_(stack), value_(value)
	{ }

	const std::string& getStack() const
	{
		return stack_;
	}

	const char* getMessage() const
	{
		return message_.c_str();
	}

	const Value& value() const
	{
		//assert(value_);
		return *value_;
	}

private:
	// This needs to be on the heap, because throw requires the object
	// be copyable, and Value is not.
	std::shared_ptr<Value> value_;
	std::string message_;
	std::string stack_;
};

extern "C" DLL_EXPORT Foo * MakeFoo();
