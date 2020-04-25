#include <memory>
#include <string>

#include "..\public.h"

class StringBuffer : public Buffer
{
public:
	StringBuffer(std::string s) : s_(std::move(s))
	{
	}
	size_t size() const override
	{
		return s_.size();
	}
	const uint8_t* data() const override
	{
		return reinterpret_cast<const uint8_t*>(s_.data());
	}

private:
	std::string s_;
};

int main()
{
	//auto f = Wrapped::MakeFoo();
	std::unique_ptr<Foo> f { MakeFoo() };
	try
	{
		f->Bar();
	}
	catch (JSError& ex)
	{
		auto val = ex.value();

		std::puts(ex.getMessage());
	}
	catch (JSIException& ex)
	{
		std::puts(ex.what());
	}
	catch (std::exception& ex)
	{
		std::puts(ex.what());
	}

	f->evaluateBuffer(std::make_unique<StringBuffer>("Something"));

	auto result = f->evaluateJavaScript("test", "url");

	std::printf("result->isNumber = %s\n", result.data_.pointer->isNumber() ? "yes" : "no");
	std::printf("exported = %d\n", f->hasDefaultImpl());

	std::puts("<< Block starts");
	{
		auto as_string = result.data_.pointer->toString();
		std::string valReconstructedCopy = as_string; // makes an explicit copy of the character buffer
		std::printf("as string = %s\n", valReconstructedCopy.c_str());

		std::string_view valROView = as_string; // only valid as long as SFINAE as_string is in scope
		std::printf("as string_view = %s\n", valROView.data()); // .data() only safe if string is known to be nul-terminated
	}
	std::puts("<< Block ends");

	result.data_.pointer->invalidate();

	std::printf("call with a lambda = %d\n", f->receivesFunction([](size_t a, size_t b)
	{
		return a * b;
	}));
}