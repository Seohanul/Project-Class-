
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <chrono>


void save(std::string fname)
{
	// fname을 읽기 모드로 연다.
	std::ifstream in(fname);

	if (!in)
		;// 오류 처리


		 // 기록할 파일을 덧붙여쓰기 모드로 연다.
	std::ofstream out("2019 STL 목금56 강의록(7).txt", std::ios::app);



	//시간 저장 
	auto tic = std::chrono::system_clock::now();


	auto time = std::chrono::system_clock::to_time_t(tic);


	std::string date = ctime(&time);

	out << std::endl << std::endl;
	out << "=========================================" << std::endl;
	out << date;
	out << "=========================================" << std::endl;
	out << std::endl;

	// fname의 모든 내용을 덧붙여 쓴다.
	// std::copy(어디부터, 어디까지 , 어디로)

	std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(out));
	// 인자 없는 생성자로 마지막을 지정함

	// 실행된 시간을 기록한다.
}

