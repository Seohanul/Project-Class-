//------------------------------------------------------------------------------------------------
// 2019�� 3�� 28�� (��)  4�� 1
//
// 2�� ���� �����̳�
//  - array
//	- vector
//	- deque
//	- list
//	- forward_list
//------------------------------------------------------------------------------------------------

#include <iostream>
#include <array>
#include <algorithm>
#include "save.h"
#include "String.h"

using namespace std;


int main()
{
	array<String, 5> a{10,20,30,40,50};

	// a�� ���� ������ �����϶�. �� �ܾ ���� ��

	for (int i = 0; i < a.size(); ++i)
		sort(a[i].getP(), a[i].getP() + a[i].getNum()); 
	for (auto Iter = a.begin(); Iter != a.end(); ++Iter)
		cout << *Iter << endl;

	save("�ҽ�.cpp");
}

