//------------------------------------------------------------------------------------------------
// 2019년 3월 29일 (금)  4주 2
//
// 2장 순차 컨테이너 (Sequence Container - homogeneous data type 동일 자료형)
//  - array : 유일하게 자료의 갯수가 고정됨
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


// 비어있는 v에 String 20개를 추가하라
// String의 값은 uid(10,70)으로 초기화하라.
// String의 문자를 오름차순으로 정렬하라.
// v를 String의 길이 오름차순으로 정렬하라.
// 결과를 화면 출력하라.

#define MAX_NUM 20
int main()
{
	uniform_int_distribution<> ui(10,70);
	default_random_engine dre;
	vector<String> v;
	
	v.reserve(MAX_NUM);
	
	for (int i = 0; i < MAX_NUM; ++i) {
		v.push_back(String{ ui(dre) });	// int 하나 있는 생성자를 알아서 찾아서 초기화
								// explicit 키워드 넣어주면 자동형변환 막아줌
	}

	for(String& s : v)		
		sort(s.getP(), s.getP() + s.getNum());

	sort(v.begin(), v.end(), [](const String & a, const String & b) {
		return a.getNum() < b.getNum();
	});

	
	for (const String& s : v)
		cout << s << endl;

	//save("소스.cpp");

}

