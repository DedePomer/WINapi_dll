#include "Header.h"
#define CSVPATH L"Users.csv"
#define DLLPATH L"Project1.dll"
typedef SCSV* (_cdecl* Parsing)(LPWSTR, LPDWORD);
typedef SCSV* (_cdecl* SearchSN) (SCSV*, LPWSTR, LPDWORD);
typedef void(_cdecl* Write) (SCSV*, LPWSTR, DWORD);
typedef void(_cdecl* AVG) (SCSV*, LPWSTR, DWORD);
typedef void(_cdecl* SUM) (SCSV*, LPWSTR, DWORD);

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HINSTANCE MyDLL;
	if (!(MyDLL = LoadLibrary(DLLPATH))) 
		return 1;
	Parsing UserList = (Parsing) GetProcAddress(MyDLL, "CreateUserArray");
	SearchSN SearchList = (SearchSN)GetProcAddress(MyDLL, "SearchSurname");
	Write WriteFille = (Write)GetProcAddress(MyDLL, "WriteCSV");
	AVG Avg = (AVG)GetProcAddress(MyDLL, "AVGage");
	SUM Sum = (SUM)GetProcAddress(MyDLL, "SUMage");

    DWORD count = 0;
    SCSV* users = UserList(CSVPATH, &count);

	LPWSTR  b = L"ова"; // фамилия
	SCSV* search = SearchList(users, b, &count);

	b = L"Search.csv";
	WriteFille(search, b, count); // файл лучше открывать в блокноте, а то непоймёте

	Avg(search, b, count);
	Sum(search, b, count);

	FreeLibrary(MyDLL);
	return 0;
}