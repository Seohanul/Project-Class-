//------------------------------------------------------------------------------------------------
// 2019년 3월 22일 (금)  3주 2
//
//	1장 읽어보기
//
//	자원을 확보하는 클래스 만들어보기
//
//
// 과제 - class String을 파일 분리하자.  #include "String.h"
//			1장 읽어보기 
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


// 연습문제 1번 (62p)
// std::string 객체로 된 배열을 정의하고, 자신이 선택한 단어들로 초기화한 후
// 반복자를 사용해 배열의 내용을 한 줄에 하나씩 출력하라.

// 연습문제 2번
// 각 원소에 transform을 적용해 모든 모음을 *로 바꿔 화면 출력하라


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


	save("소스.cpp");

}

