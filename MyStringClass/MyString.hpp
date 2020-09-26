#pragma once
#include <ostream>

class MyString
{
	std::size_t length;
	char* data;

	MyString(const char* x, const char* y, const std::size_t length);

public:

	MyString();

	MyString(const char* c);
	
	const char* c_str() const;

	~MyString()= default;

	MyString Concatenate(const MyString str) const;

	std::size_t GetLength() const;

	friend std::ostream& operator <<(std::ostream& os, const MyString& str);
};

