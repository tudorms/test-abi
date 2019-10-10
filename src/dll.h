struct Foo
{
protected:
	Foo() = default;

public:
	virtual ~Foo() = default;
	virtual void Bar() = 0;
};

extern "C" Foo *MakeFoo();
