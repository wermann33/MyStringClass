#include <iostream>


namespace MyString
{
	class MyString
	{
	private:

		std::size_t length; //length without '\0'!
		char* data;

		//Private constructor to help to concatenate.
		MyString(const char* x, const char* y, const std::size_t length);

	public:

		//simple constructor
		MyString();

		//const char* constructor
		MyString(const char* c);

		//copy constructor
		MyString(const MyString& other);

		//copy assignment operator
		MyString& operator= (const MyString& other);

		//move constructor
		MyString(MyString&& other) noexcept;

		//move assignment operator
		MyString& operator= (MyString&& other) noexcept;

		//destructor
		~MyString();

		//function to test move assignment operator
		void MoveString(MyString& str1, MyString& str2) const;

		//c_str
		const char* c_str() const;

		//concatenator
		MyString Concatenate(const MyString str) const;

		//returns length without '\0'
		std::size_t GetLength() const;

		//friend 4 cout operator overload <<
		friend std::ostream& operator <<(std::ostream& os, const MyString& str);
	};

	std::size_t length; //length without '\0'!
	char* data;

	MyString::MyString(const char* x, const char* y, const std::size_t new_length) : length(new_length) //initialise length to new_length(length of both the concatenated strings)
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

	MyString::MyString()
	{
		length = 0;
		data = new char[1];
	}

	MyString::MyString(const char* c)
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
	MyString::MyString(const MyString& other) : length(other.length)
	{
		data = new char[length + 1];
		for (std::size_t i = 0; i < length; i++)
			data[i] = other.c_str()[i];
		data[length] = '\0';
	}

	//copy assignment operator
	MyString& MyString::operator= (const MyString& other) //length not assigned in initialization list, because may not be necessary, if self-assignment
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
	MyString::MyString(MyString&& other) noexcept : length(other.length), data(other.data) //noexcept to assure move constructor gets used => wont throw any exceptions		
	{																				//length assignment may not be necessary
		other.data = nullptr;
		other.length = 0;
	}

	//move assignment operator
	MyString& MyString::operator= (MyString&& other) noexcept //noexcept again like in move constructor
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
	MyString::~MyString()
	{
		delete[] data;
	}

	const char* MyString::c_str() const
	{
		return data;
	}

	MyString MyString::Concatenate(const MyString str) const
	{
		const std::size_t new_length = length + str.GetLength(); //adding lengths, to know length, of the new string
		return MyString(data, str.c_str(), new_length); //private constructor
	}

	std::size_t MyString::GetLength() const
	{
		return length;
	}

	//Move Function for Testing move assignment operator

	void MyString::MoveString(MyString& str1, MyString& str2) const
	{
		str1 = std::move(str2);
	}

	//cout(<<) operator override  
	std::ostream& operator<<(std::ostream& os, const MyString& str)
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
}

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

	return 0;
}