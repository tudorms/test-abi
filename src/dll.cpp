#include <cstdio>
#include <cstdlib>
#include <new>

#include "dll.h"

class Zot : public Foo
{
	~Zot() {}
	void Bar() override{};
};

void *operator new(std::size_t sz)
{
	std::printf("global op new called, size = %zu\n", sz);
	void *ptr = std::malloc(sz);
	if (ptr)
		return ptr;
	else
		throw std::bad_alloc{};
}
void operator delete(void *ptr) noexcept
{
	std::puts("global op delete called");
	std::free(ptr);
}

Foo *MakeFoo()
{
	return new Zot();
}