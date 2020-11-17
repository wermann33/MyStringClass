#include <iostream>
#include <cassert>
#include <algorithm>
#include <iterator>

namespace MyString
{
	class MyString
	{
	private:

		std::size_t length; //length without '\0'!
		char* data;

		//Private constructor to help to concatenate.
		MyString(const char* x, const char* y, const std::size_t new_length) : length(new_length) //initialise length to new_length(length of both the concatenated strings)
		{
			data = new char[new_length + 1]; //+1 for '\0'

			std::size_t idx1 = 0;

			auto concat = [&idx1, this](const char* str) //lambda 4 concat the strings by index
			{
				std::size_t idx2 = 0;
				while (str[idx2] != '\0')
				{
					data[idx1++] = str[idx2++];
				}
			};

			concat(x);
			concat(y);
			data[new_length] = '\0'; //adding the final '\0' at end of new MyString
		}

	public:

		//simple constructor
		MyString()
		{
			length = 0;
			data = new char[1];
		}

		//const char* constructor
		MyString(const char* c)
		{
			if (c == nullptr) //checking 4 nullptr, not sure, if necessary
			{
				length = 0;
				data = new char[1];
				data[0] = '\0';
			}
			else //"copy" const char* c to char* data
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

		//copy constructor
		MyString(const MyString& other) : length(other.length)
		{
			data = new char[length + 1];
			for (std::size_t i = 0; i < length; i++)
				data[i] = other.c_str()[i];
			data[length] = '\0';
		}

		//copy assignment operator
		MyString& operator= (const MyString& other) //length not assigned in initialization list, because may not be necessary, if self-assignment
		{
			//checking for self-assignment
			if (this == &other)
			{
				return *this;
			}

			length = other.length;
			for (std::size_t i = 0; i < length; i++)
				data[i] = other.c_str()[i];
			data[length] = '\0';
			return *this;
		}

		//move constructor
		MyString(MyString&& other) noexcept : length(other.length), data(other.data) //noexcept to assure move constructor gets used => wont throw any exceptions		
		{																				//length assignment may not be necessary
			other.data = nullptr;
			other.length = 0;
		}

		//move assignment operator
		MyString& operator= (MyString&& other) noexcept //noexcept again like in move constructor
		{
			//checking for self-assignment
			if (this == &other)
			{
				return *this;
			}
			length = other.length; //again: may not be necessary
			data = other.data;
			other.data = nullptr;
			other.length = 0;
			return *this;
		}

		//destructor
		~MyString()
		{
			delete[] data;
		}

		//function to test move assignment operator
		void MoveString(MyString& str1, MyString& str2) const
		{
			str1 = std::move(str2);
		}

		//c_str
		const char* c_str() const
		{
			return data;
		}

		//concatenator
		MyString Concatenate(const MyString str) const
		{
			const std::size_t new_length = length + str.GetLength(); //adding lengths, to know length, of the new string
			return MyString(data, str.c_str(), new_length); //private constructor
		}

		//returns length without '\0'
		std::size_t GetLength() const
		{
			return length;
		};

		//friend 4 cout operator overload <<
		friend std::ostream& operator<<(std::ostream& os, const MyString& str)
		{
			if (str.data == nullptr)
			{
				os << "nullptr";
				return os;
			}
			std::size_t idx = 0;
			while (str.data[idx] != '\0')
			{
				os << str.data[idx++];
			}
			return os;
		}

		// operator+= overload with argument const Mystring&
		MyString& operator+= (const MyString& other)
		{
			const std::size_t len = length + other.length; //length of updated string
			auto* str = new char[len + 1]; //+1 for '\0'

			for (std::size_t j = 0; j < length; j++) //copying MyString 1
				str[j] = data[j];

			for (std::size_t i = 0; i < other.length; i++) //copying MyString 2
				str[i + length] = other.data[i];

			str[len] = '\0';

			//updating and returning string
			delete data;
			length = len;
			data = str;
			return *this;
		}

		// operator+= overload with argument const char*&
		MyString& operator+= (const char*& other)
		{
			std::size_t other_length = 0;
			while (other[other_length] != '\0') other_length++; //getting length of char*

			const std::size_t len = length + other_length; //length of updated string
			auto* str = new char[len + 1]; //+1 for '\0'

			for (std::size_t j = 0; j < length; j++) //copying MyString 1
				str[j] = data[j];

			for (std::size_t i = 0; i < other_length; i++) //copying char*
				str[i + length] = other[i];

			str[len] = '\0';
			//updating and returning string
			delete data;
			length = len;
			data = str;
			return *this;
		}

		friend MyString operator+ (const MyString& str1, const MyString& str2) //friend, because assigned to a new MyString (2 arguments needed)
		{
			MyString newString = str1.Concatenate(str2);
			return  newString;  //using the +=overload instead would cause unnecessary copying
		}

		friend MyString operator+ (const MyString& str1, const char*& str2) //friend, because assigned to a new MyString (2 arguments needed)
		{
			MyString newString = str1;
			newString += str2;  //+= useful here, no additional copy
			return newString;
		}

		friend MyString operator+ (const char*& str1, const MyString& str2) //friend, because assigned to a new MyString (2 arguments needed)
		{
			MyString newString = MyString(str1).Concatenate(str2);
			return newString;
		}

		//conversion function to const char*
		operator const char* () const
		{
			return this->c_str();
		}


		class Iterator
		{
		public:

			Iterator(char* charPointer = nullptr) :_charPointer(charPointer) { }  //Initzialise charPointer

			bool operator!=(const Iterator& iterator) const
			{
				return _charPointer != iterator._charPointer;
			}
			void operator++()
			{
				++_charPointer;
			}
			char& operator*() const
			{
				return *_charPointer;
			}

		private:
			char* _charPointer;
		};

		Iterator begin() const //const = no need to manipulate Class
		{
			return { data };
		}
		Iterator end() const //const = no need to manipulate Class
		{
			return { data + GetLength() };
		}
	};
}

//necassary for gcc to use std::find
template<>
struct std::iterator_traits<MyString::MyString::Iterator>
{
	typedef std::ptrdiff_t difference_type;
	typedef char value_type;
	typedef const char& reference; 
	typedef const char* pointer;
	typedef std::input_iterator_tag iterator_category;
};

int main()
{
	//Testing constructor
	MyString::MyString str1("Hallo");

	auto* str2 = new MyString::MyString("Welt!");

	const MyString::MyString str3(" kranke Welt!");

	std::cout << str1 << ' ' << str2->c_str() << std::endl;

	//Testing c_str()
	for (std::size_t i = 0; i < str1.GetLength(); i++)
		std::cout << str1.c_str()[i] << ' ';
	std::cout << std::endl;

	//Testing Concatenate()
	MyString::MyString str4 = str1.Concatenate(" Welt");

	std::cout << str4 << "[Length: " << str4.GetLength() << ']' << std::endl;

	//Testing copy constructor
	MyString::MyString myString("Ole");
	MyString::MyString otherString(myString); //jumps into check copy constructor as expected
	auto* newString = new MyString::MyString(myString); //jumps into check copy constructor as expected

	std::cout << myString << ", " << otherString << ", " << newString->c_str() << std::endl;

	//Testing copy assignment operator
	MyString::MyString cao1("Servus");
	MyString::MyString cao2(" Kasperl!");

	std::cout << cao1 << cao2 << std::endl;

	cao2 = cao1; //jumps into copy assignment operator as expected

	std::cout << cao2 << std::endl;

	//Testing copy constructor self-assignment
	MyString::MyString cao3("Krawutzikaputzi");
	//cao3 = cao3; //jumps into check self-assignment as expected (commented out because generates a warning in Clang compiler)
	std::cout << cao3 << std::endl;


	//Testing move constructor
	MyString::MyString mc1("Halligalli");

	MyString::MyString mc2 = std::move(mc1); //jumps into move constructor as expected

	std::cout << mc2 << std::endl;


	//Testing move assignment operator
	MyString::MyString mao1("Beweg dich!");
	MyString::MyString mao2 = nullptr;

	mao2.MoveString(mao2, mao1); //jumps into check move assignment operator as expected

	std::cout << mao2 << "[Length: " << mao2.GetLength() << ']' << std::endl;

	std::cout << mao1 << ", Length: " << mao1.GetLength() << std::endl; //returns "nullptr", and length 0


	//Testing operator+= (const MyString& s)
	MyString::MyString s1("Hello ");
	const MyString::MyString s2("World");
	s1 += s2;
	std::cout << s1 << std::endl;

	//Testing operator+= (const char*& s)
	MyString::MyString s3("Goodbye ");
	const char* s4 = "World";
	const char* s5 = "!";
	s3 += s4;
	s3 += s5;
	std::cout << s3 << std::endl;

	//Testing operator+ 2 MyStrings
	MyString::MyString s6("Dibedib");
	MyString::MyString s7("edam");
	MyString::MyString s8 = s6 + s7;
	std::cout << s8 << std::endl;
	std::cout << s6 << std::endl; //is still "Dibedib"

	//Testing operator+ MyString + char*&
	const char* c1 = "edam";
	MyString::MyString s9 = s6 + c1;
	std::cout << s9 << std::endl;
	std::cout << s6 << std::endl; //is still "Dibedib"

	//Testing operator+ char*& + MyString
	const char* c2 = "Dibedib";
	MyString::MyString s10 = c2 + s7;;
	std::cout << s10 << std::endl;
	std::cout << s6 << std::endl; //is still "Dibedib"

	//Testing conversion function to const char*
	puts(s10);

	//Testing Iterator
	const MyString::MyString test("Hello World");
	for (auto it = test.begin(); it != test.end(); ++it) { std::cout << *it << std::endl; }
	std::cout << std::endl;
	for (auto& it : test) { std::cout << it << std::endl; }
	std::cout << std::endl;
	assert((std::find(test.begin(), test.end(), 'W') != test.end()) == 1);
	std::cout << (std::find(test.begin(), test.end(), 'W') != test.end()) << std::endl;
	assert((std::find(test.begin(), test.end(), 'X') != test.end()) == 0);
	std::cout << (std::find(test.begin(), test.end(), 'X') != test.end()) << std::endl;
	assert(*test.begin() == 'H');
	std::cout << *test.begin() << std::endl;

	return 0;
}
