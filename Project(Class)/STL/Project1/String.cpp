#include"String.h"

#include <iostream>
#include <random>
#include <algorithm>
// 정수를 생성자의 인자로 받아 그 숫자만큼(byte) HEAP에서 메모리를 할당한다.
// 메모리의 내용은 'a' ~ 'z' 사이의 임의의 소문자로 채운다
// 다음 main이 문제없이 실행되도록 하자. 


// String은 자원을 할당하는 클래스이다.
// 1. 생성 시 new로 자원할당
// 2. 소멸 시 delete로 해제 
// 3. 복사 생성자에서 깊은 복사를 한다. 
// 4. 할당 연산자에서 깊은 복사를 한다. 

std::uniform_int_distribution<> ui('a', 'z');
std::default_random_engine dre;

String::String(int size) : num(size)
{
	
	p = new char[size];
	for (int i = 0; i < size; ++i)
		p[i] = ui(dre);
}

String::String(const String & other) : num{ other.num }
{
	p = new char[num];
	memcpy(p, other.p, num);
}

String::~String()
{
	//std::cout << "~String() - " << this << std::endl;
	delete[] p;
}

String & String::operator=(const String & other)
{
	if (this == &other)
		return *this;

	delete[] p;
	num = other.num;
	p = new char[num];
	memcpy(p, other.p, num);

}

void String::setNum(int n)
{
	num = n;
	if (!p)
		delete[] p;


	p = new char[num];
	for (int i = 0; i < num; ++i)
		p[i] = ui(dre);
}

int String::getNum() const
{
	return num;
}

char * String::getP() const
{
	return p;
}

std::ostream& operator<<(std::ostream& os, const String& s)
{
	for (int i = 0; i < s.num; ++i)
		os << s.p[i];
	return os;
}



