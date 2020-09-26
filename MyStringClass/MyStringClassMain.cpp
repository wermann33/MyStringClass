#include "MyString.hpp"
#include <iostream>


int main()
{
	MyString str1("Hallo");

	auto* str2 = new MyString("Welt!");

	const MyString str3(" kranke Welt!");

	std::cout << str1 << ' ' << str2->c_str() << std::endl;

	for (std::size_t i = 0; i < str1.GetLength(); i++)
		std::cout << str1.c_str()[i] << ' ';
	std::cout << std::endl;

	MyString str4 = str1.Concatenate(str3);

	std::cout << str4 << "[Length: " << str4.GetLength() << ']' << std::endl;

	return 0;
}

