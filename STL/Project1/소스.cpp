//------------------------------------------------------------------------------------------------
// 2019�� 3�� 29�� (��)  4�� 2
//
// 2�� ���� �����̳� (Sequence Container - homogeneous data type ���� �ڷ���)
//  - array : �����ϰ� �ڷ��� ������ ������
//	- vector : dynamic array
//	- deque
//	- list
//	- forward_list
//------------------------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "save.h"
#include "String.h"

using namespace std;


// ����ִ� v�� String 20���� �߰��϶�
// String�� ���� uid(10,70)���� �ʱ�ȭ�϶�.
// String�� ���ڸ� ������������ �����϶�.
// v�� String�� ���� ������������ �����϶�.
// ����� ȭ�� ����϶�.

#define MAX_NUM 20
int main()
{
	uniform_int_distribution<> ui(10,70);
	default_random_engine dre;
	vector<String> v;
	
	v.reserve(MAX_NUM);
	
	for (int i = 0; i < MAX_NUM; ++i) {
		v.push_back(String{ ui(dre) });	// int �ϳ� �ִ� �����ڸ� �˾Ƽ� ã�Ƽ� �ʱ�ȭ
								// explicit Ű���� �־��ָ� �ڵ�����ȯ ������
	}

	for(String& s : v)		
		sort(s.getP(), s.getP() + s.getNum());

	sort(v.begin(), v.end(), [](const String & a, const String & b) {
		return a.getNum() < b.getNum();
	});

	
	for (const String& s : v)
		cout << s << endl;

	//save("�ҽ�.cpp");

}

