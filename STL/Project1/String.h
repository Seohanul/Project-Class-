#pragma once


#include <iostream>
#include <random>
class String
{

public:

	String() = default;

	String(int size);

	String(const String&);


	~String();

	friend std::ostream& operator<<(std::ostream& os, const String& s);

	String& operator= (const String& other);

	void setNum(int n);

	int getNum() const;

	char* getP() const;
private:


	char * p = nullptr;
	int num{ 0 };
};
