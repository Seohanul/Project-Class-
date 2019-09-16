
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <chrono>


void save(std::string fname)
{
	// fname�� �б� ���� ����.
	std::ifstream in(fname);

	if (!in)
		;// ���� ó��


		 // ����� ������ ���ٿ����� ���� ����.
	std::ofstream out("2019 STL ���56 ���Ƿ�(7).txt", std::ios::app);



	//�ð� ���� 
	auto tic = std::chrono::system_clock::now();


	auto time = std::chrono::system_clock::to_time_t(tic);


	std::string date = ctime(&time);

	out << std::endl << std::endl;
	out << "=========================================" << std::endl;
	out << date;
	out << "=========================================" << std::endl;
	out << std::endl;

	// fname�� ��� ������ ���ٿ� ����.
	// std::copy(������, ������ , ����)

	std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(out));
	// ���� ���� �����ڷ� �������� ������

	// ����� �ð��� ����Ѵ�.
}

