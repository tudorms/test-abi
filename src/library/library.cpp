#include <cstdio>
#include <cstdlib>
#include <new>

#include "..\public.h"

class JsiValueImpl : public JsiValue
{
public:
	~JsiValueImpl()
	{
		std::puts("JsiValueImpl destructor called");
	}

	bool isNumber() const noexcept
	{
		return false;
	}

	void invalidate() override
	{
		delete this;
	}

	abi::string_view_ptr toString() override
	{
		std::string value { "I am a somewhat long string that is expensive to compute!" };
		return abi::string_view_ptr { new abi::string_view_impl { std::move(value) } };
	}
};

class Zot : public Foo
{
	~Zot()
	{}

	void Bar() override
	{
		std::puts("Zot::Bar called");

		//throw std::exception("WHAT!?");
		//throw JSINativeException("WHAT!?");
		auto val = std::make_shared<Value>();
		val->kind_ = Value::NumberKind;
		val->data_.number = 42;
		throw JSError(val, "message", "stack");
	}

	Value evaluateJavaScript(const char* buffer, const char* sourceURL) override
	{
		Value value;
		value.data_.pointer = new JsiValueImpl();
		return value;
	}

	int hasDefaultImpl() override
	{
		return 43;
	}

	/*size_t receivesFunction(HostFunctionType func) override
	{
		return func(1, 1);
	}*/

	size_t receivesCFunction(CHostFunctionType* funcPointer, void* pv) override
	{
		// reconstructedFunc cannot be copied / used after this function returns because pv is no longer valid (and we cannot take onwership without an ABI wrapper)
		HostFunctionType reconstructedFunc = [funcPointer, pv](size_t a, size_t b) -> size_t
		{
			return funcPointer(pv, a, b);
		};

		return reconstructedFunc(2, 2);
	}
};

void* operator new(std::size_t sz)
{
	void* ptr = std::malloc(sz);
	std::printf("global op new called, size = %zu, addr = %p\n", sz, ptr);
	if (ptr)
		return ptr;
	else
		throw std::bad_alloc {};
}

void operator delete(void* ptr) noexcept
{
	std::printf("global op delete called, addr = %p\n", ptr);
	std::free(ptr);
}

Foo* MakeFoo()
{
	return new Zot();
}