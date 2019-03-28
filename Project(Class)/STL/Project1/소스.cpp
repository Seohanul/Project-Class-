//------------------------------------------------------------------------------------------------
// 2019년 3월 28일 (목)  4주 1
//
// 2장 순차 컨테이너
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

	// a를 사전 순으로 정렬하라. 각 단어가 사전 순

	for (int i = 0; i < a.size(); ++i)
		sort(a[i].getP(), a[i].getP() + a[i].getNum()); 
	for (auto Iter = a.begin(); Iter != a.end(); ++Iter)
		cout << *Iter << endl;

	save("소스.cpp");
}

