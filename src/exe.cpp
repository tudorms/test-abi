#include "dll.h"

int main()
{
	auto f = MakeFoo();
	f->Bar();
	delete f;
}