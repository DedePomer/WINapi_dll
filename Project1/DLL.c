#include  "..//Header.h"

BOOL WINAPI DllMain(HINSTANCE hlnstDll, DWORD dwReason, LPVOID IpReserved)
{
    BOOL bAllWentWell = TRUE;
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    if (bAllWentWell)
        return TRUE;
    else
        return FALSE;
}

LPWSTR Reading(LPWSTR path)
{
    DWORD d = 0;
    HANDLE File = CreateFile(path, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (File == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"Файла нет", L"Ошибка", MB_OK | MB_ICONHAND);
        exit(-1);
    }
    LPWSTR ReadString = (LPWSTR)calloc(100000, sizeof(WCHAR));
    ReadFile(File, ReadString, 100000 * sizeof(WCHAR), &d, NULL);
    CloseHandle(File);
    ReadString[d / 2] = '\n';
    ReadString[d / 2 + 1] = '\0';
    return ReadString;

}

DWORD CountUsers(LPWSTR str)
{
    DWORD count = 0;
    for (size_t i = 0; i < wcslen(str); i++)
    {
        if (str[i] == L'\n') count++;
    }
    return count;
}

DWORD LpwstrToDword(LPWSTR str)
{
    DWORD dw = 0;
    for (size_t i = 0; i < wcslen(str); i++)
    {
        dw += (str[i] - '0');
        dw *= 10;
    }
    return dw / 10;
}

DWORD CountSearch(SCSV* s, LPWSTR sn, DWORD count)
{
    DWORD a = 0, z = 0, ind = 0;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < wcslen(s[i].surname); j++)
        {
            if (s[i].surname[j] == sn[z] && z == 0)
            {
                z++;
                ind = j + 1;
            }
            else if (j == ind && s[i].surname[j] == sn[z])
            {
                z++;
                ind = j + 1;
            }
        }
        if (z == wcslen(sn))
        {
            a++;
        }
        z = 0; ind = 0;
    }
    return a;
}

SCSV* ListSearch(SCSV* s, SCSV* s1, LPWSTR sn, DWORD count)
{
    DWORD a = 0, z = 0, ind = 0, k = 0;
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < wcslen(s[i].surname); j++)
        {
            if (s[i].surname[j] == sn[z] && z == 0)
            {
                z++;
                ind = j + 1;
            }
            else if (j == ind && s[i].surname[j] == sn[z])
            {
                z++;
                ind = j + 1;
            }
        }
        if (z == wcslen(sn))
        {
            s1[k] = s[i];
            k++;
        }
        z = 0; ind = 0;
    }
    return s1;
}

extern "C" __declspec(dllexport) SCSV * CreateUserArray(LPWSTR, LPDWORD);
StructcCSV* CreateUserArray(LPWSTR path, LPDWORD n)
{
    LPWSTR str = Reading(path);
    DWORD count = CountUsers(str);
    StructcCSV* users = (StructcCSV*)calloc(count, sizeof(StructcCSV));
    DWORD poz = 0, stroke = 0, ind = 0;
    LPWSTR strvozr = (LPWSTR)calloc(3, sizeof(WCHAR));
    for (int i = 1; i < wcslen(str); i++)
    {
        if (str[i] == '\n')
        {
            strvozr = (LPWSTR)calloc(3, sizeof(WCHAR));
            stroke++;
            poz = 0;
            ind = 0;
        }
        else
        {
            if (str[i] == L';')
            {
                poz++;
                ind = 0;
            }
            else
            {
                if (poz == 0) users[stroke].surname[ind] = str[i];
                if (poz == 1) users[stroke].name[ind] = str[i];
                if (poz == 2) users[stroke].secondname[ind] = str[i];
                if (poz == 3)
                {
                    if (str[i] == L'\r')
                    {
                        strvozr[ind] = L'\0';
                        users[stroke].age = LpwstrToDword(strvozr);
                        free(strvozr);
                    }
                    else strvozr[ind] = str[i];
                }
                ind++;
            }
        }
    }
    *n = count;
    return users;
}

extern "C" __declspec(dllimport) SCSV * SearchSurname(SCSV*, LPWSTR, LPDWORD);
StructcCSV* SearchSurname(SCSV* s, LPWSTR sn, LPDWORD count)
{
    int countS = CountSearch(s, sn, *count);
    SCSV* SearchSN = (SCSV*)calloc(countS, sizeof(SCSV));
    SearchSN = ListSearch(s, SearchSN, sn, *count);
    *count = countS;
    return SearchSN;
}

extern "C" __declspec(dllimport) void WriteCSV(SCSV*, LPWSTR, DWORD);
void WriteCSV(SCSV* s, LPWSTR nameF, DWORD count)
{
    DWORD a;
    LPWSTR str;
    HANDLE File = CreateFile(nameF, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    for (size_t i = 0; i < count; i++)
    {
        str = (LPWSTR)calloc(150, 2);
        wsprintfW(str, L"%IS;%IS;%IS;%d\r\n", s[i].surname, s[i].name, s[i].secondname, s[i].age);
        WriteFile(File, str, wcslen(str) * 2, &a, NULL);
        free(str);
    }
    CloseHandle(File);
}

extern "C" __declspec(dllimport) void AVGage(SCSV*, LPWSTR, DWORD);
void AVGage(SCSV* s, LPWSTR nameF, DWORD count)
{
    DWORD avg = 0, a;
    for (int i = 0; i < count; i++)
    {
        avg += s[i].age;
    }
    avg = avg / count;
    LPWSTR str = (LPWSTR)calloc(20, sizeof(WCHAR));
    HANDLE File = CreateFile(nameF, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    wsprintfW(str, L"Средний возраст %d\r\n", avg);
    SetFilePointer(File, 0, NULL, FILE_END);
    WriteFile(File, str, wcslen(str) * 2, &a, NULL);
    CloseHandle(File);
}

extern "C" __declspec(dllimport) void SUMage(SCSV*, LPWSTR, DWORD);
void SUMage(SCSV* s, LPWSTR nameF, DWORD count)
{
    DWORD avg = 0, a;
    for (int i = 0; i < count; i++)
    {
        avg += s[i].age;
    }
    LPWSTR str = (LPWSTR)calloc(50, sizeof(WCHAR));
    HANDLE File = CreateFile(nameF, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    wsprintfW(str, L"Сумма возрастов %d\r\nКол-во человек %d\r\n", avg, count);
    SetFilePointer(File, 0, NULL, FILE_END);
    WriteFile(File, str, wcslen(str) * 2, &a, NULL);
    CloseHandle(File);
}