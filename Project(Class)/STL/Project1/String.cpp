#include"String.h"

#include <iostream>
#include <random>
#include <algorithm>
// ������ �������� ���ڷ� �޾� �� ���ڸ�ŭ(byte) HEAP���� �޸𸮸� �Ҵ��Ѵ�.
// �޸��� ������ 'a' ~ 'z' ������ ������ �ҹ��ڷ� ä���
// ���� main�� �������� ����ǵ��� ����. 


// String�� �ڿ��� �Ҵ��ϴ� Ŭ�����̴�.
// 1. ���� �� new�� �ڿ��Ҵ�
// 2. �Ҹ� �� delete�� ���� 
// 3. ���� �����ڿ��� ���� ���縦 �Ѵ�. 
// 4. �Ҵ� �����ڿ��� ���� ���縦 �Ѵ�. 

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



