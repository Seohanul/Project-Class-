//------------------------------------------------------------------------------------------------
// 2019�� 3�� 22�� (��)  3�� 2
//
//	1�� �о��
//
//	�ڿ��� Ȯ���ϴ� Ŭ���� ������
//
//
// ���� - class String�� ���� �и�����.  #include "String.h"
//			1�� �о�� 
//------------------------------------------------------------------------------------------------

#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include <string>
#include <iterator>
#include "save.h"
#include "String.h"
using namespace std;


// �������� 1�� (62p)
// std::string ��ü�� �� �迭�� �����ϰ�, �ڽ��� ������ �ܾ��� �ʱ�ȭ�� ��
// �ݺ��ڸ� ����� �迭�� ������ �� �ٿ� �ϳ��� ����϶�.

// �������� 2��
// �� ���ҿ� transform�� ������ ��� ������ *�� �ٲ� ȭ�� ����϶�


int main()
{
	string s[5]{"kill", "die", "monkey" ,"changja", "hu"}; //RAII

	transform(begin(s), end(s), ostream_iterator<string>(cout, "\n"), [](string& str) {
		for (char& c : str) {
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
				c = '*';
		}
		return str;
	});	


	save("�ҽ�.cpp");

}

