#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h> 


struct StructcCSV
{
	WCHAR  surname[30];
	WCHAR  name[30];
	WCHAR  secondname[30];
	DWORD  age;
};
typedef struct StructcCSV SCSV;
