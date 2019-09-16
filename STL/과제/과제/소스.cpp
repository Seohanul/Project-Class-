#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <streambuf>
using namespace std;


void CharAdder(char* LeftDigit, char* RightDigit, char A, char B, char R)
{
	char Result = A - '0' + B - '0' + R - '0';
	*RightDigit = (Result % 10) + '0';
	*LeftDigit = (Result / 10) + '0';
}

string ProcessAdd(const string& longstr, const string& shortstr)
{
	string Result;
	Result.reserve(longstr.size() + 1);

	auto LongIter = longstr.rbegin();
	char R = '0';

	for (auto ShortIter = shortstr.rbegin(); ShortIter != shortstr.rend(); ++ShortIter)
	{
		char Left, Right;
		CharAdder(&Left, &Right, *ShortIter, *LongIter, R);
		R = Left;
		Result.push_back(Right);
		++LongIter;
	}

	for (auto RIter = LongIter; RIter != longstr.rend(); ++RIter)
	{
		char Left, Right;
		CharAdder(&Left, &Right, *RIter, '0', R);
		R = Left;
		Result.push_back(Right);
	}

	if (R != '0')
		Result.push_back(R);

	reverse(Result.begin(), Result.end());

	return Result;
}

string Add(const string& A, const string& B)
{
	if (A.length() > B.length())
		return ProcessAdd(A, B);
	else
		return ProcessAdd(B, A);
}

int main()
{

	ifstream File1("op1.txt");
	ifstream File2("op2.txt");
	string str1((std::istreambuf_iterator<char>(File1)), istreambuf_iterator<char>());
	string str2((std::istreambuf_iterator<char>(File2)), istreambuf_iterator<char>());

	cout << "  " << str1 << "\n+ " << str2 << "\n\n= " << Add(str1, str2) << "\n";
}