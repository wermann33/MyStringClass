#pragma once
#include <ostream>

class MyString
{
	std::size_t length; //length without '\0'!
	char* data;

	//Private constructor to help to concatenate.
	MyString(const char* x, const char* y, const std::size_t length);

public:

	//simple constructor
	MyString();

	//const char* constructor
	MyString(const char* c);
	
	const char* c_str() const;

	//destructor
	~MyString();

	MyString Concatenate(const MyString str) const;

	std::size_t GetLength() const;

	//friend 4 cout operator overload <<
	friend std::ostream& operator <<(std::ostream& os, const MyString& str);
};

