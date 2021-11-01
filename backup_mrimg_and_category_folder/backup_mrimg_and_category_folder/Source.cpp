#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include <filesystem>
#include <string>
#include <cstdio>
#include <chrono>
#include <ctime>
#include <locale>
#include <codecvt>
#include <map>
#include <tuple>
using namespace std;
namespace fs = std::filesystem;

//C++17!!!!!!
const std::string currentDateTime();
const std::pair<string, string> get_bak_year_month();

int wmain(int argc, wchar_t* argv[]) {

	if (argc != 3) {
		cout << "command error must be 3 parameters" << endl;
		return 0;
	}

	//std::wstring img_path = L"D:\\C_bak/;"
	std::wstring img_path = argv[1];
	if (img_path.back() != L'\\' && img_path.back() != L'/') {
		img_path.push_back(L'\\');
	}
	cout << "img store path:";
	wcout << img_path << endl;


	//std::wstring upload_bat_path = L"D:\\C_bak_utils\\upload.bat ";
	std::wstring upload_bat_path = argv[2];	
	cout << "upload.bat path:";
	wcout << upload_bat_path << endl;

	upload_bat_path += L" ";

	 

	
	map<string,vector<string>> all_img_id;
	for (const auto& entry : std::filesystem::directory_iterator(img_path)) {
		if (entry.is_directory()) continue;
		string filename = entry.path().filename().string();
		string img_id = "";
		bool get_dash = false;
		for (auto i : filename) {
			if (i == '-') {
				get_dash = true;
				break;
			}
			img_id.push_back(i);
		}
		if (get_dash) {
			all_img_id[img_id].push_back(filename);
		}
	}
	if (all_img_id.empty()) return 0;



	wstring command;
	std::pair<string, string> year_month = get_bak_year_month();
	for (auto& img_id : all_img_id) {		
		string foldername = year_month.first + '\\' + img_id.first +'_' + year_month.first + '-' + year_month.second;
		cout << foldername << endl;		
		for (auto& filename : img_id.second) {
			cout <<'\t'<< filename << endl;
			command += (upload_bat_path + img_path + wstring(filename.begin(),filename.end()) + L" " + wstring(foldername.begin(), foldername.end())) + L" && ";
		}		
	}
	command.pop_back();//空白
	command.pop_back();//&
	command.pop_back();//&
	command.pop_back();//空白
	std::wcout << command << endl;


	STARTUPINFO info = { sizeof(info) };
	PROCESS_INFORMATION processInfo;
	LPTSTR command_LP = const_cast<LPWSTR>(command.c_str());
	
	ZeroMemory(&info, sizeof(info));
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (CreateProcess(NULL, command_LP, NULL, NULL, FALSE, 0, NULL, NULL, &info, &processInfo))
	{
		cout << "start upload" << endl;
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
	}
	else {
		cerr << "failed to create process :"<<GetLastError() << endl;
	}
	TerminateProcess(processInfo.hProcess, 300);
	//终止  
	ExitProcess(1001);

	cout << "finished" << endl;
	return 0;
}

const std::pair<std::string, std::string> get_bak_year_month() {//每個月的第一個星期一前 自動算成上一個月
	time_t     now = time(0);
	struct tm  tstruct;
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format

	int year = tstruct.tm_year + 1900;
	int month = tstruct.tm_mon + 1;
	int mday = tstruct.tm_mday;//day of the month 1~31
	int wday = tstruct.tm_wday;//days since Sunday  星期日 => 0,星期一 => 1,星期二 => 2 ...

	if (wday == 0) {//星期日算成7
		wday = 7;
	}
	if (wday > mday) {//修正成上一個月，若星期幾數大於幾號 則代表還沒到該月的第一個星期一，算入上個月
		if (month == 1) {//1月 退回去年12月
			--year;
			month = 12;
		}
		else {
			month--;
		}
	}
	return make_pair(to_string(year), to_string(month));
}


const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m", &tstruct);

	return buf;
}