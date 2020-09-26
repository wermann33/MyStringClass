#include "MyString.hpp"

MyString::MyString()
{
	length = 0;
	data = new char[1];
}

MyString::MyString(const char* x, const char* y, const std::size_t new_length) : length(new_length)
{
	data = new char[new_length + 1];

	std::size_t idx1 = 0;

	auto concat = [&idx1, this](const char* str)
	{
		std::size_t idx2 = 0;
		while (str[idx2] != '\0')
		{
			data[idx1++] = str[idx2++];
		}
	};

	concat(x);
	concat(y);
	data[new_length] = '\0';
}

MyString::MyString(const char* c)
{
	if (c == nullptr)
	{
		length = 0;
		data = new char[1];
		data[0] = '\0';
	}
	else
	{
		std::size_t n = 0;
		while (c[n] != '\0') n++;
		length = n;
		data = new char[n + 1];
		for (std::size_t i = 0; i < n; i++)
			data[i] = c[i];
		data[n] = '\0';
	}
}

const char* MyString::c_str() const
{
	return data;
}

MyString MyString::Concatenate(const MyString str) const
{
	const std::size_t new_length = length + str.GetLength();
	return MyString(data, str.c_str(), new_length);
}

std::size_t MyString::GetLength() const
{
	return length;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
	std::size_t idx = 0;
	while (str.data[idx] != '\0')
	{
		os << str.data[idx++];
	}
	return os;
}