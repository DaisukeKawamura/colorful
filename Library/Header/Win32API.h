#pragma once
#include <Windows.h>

class Win32API
{
	/*メンバ変数*/
public:
WNDCLASSEX w;
HWND hWnd{};
#ifdef UNICODE
LPCWSTR className; //クラス名
LPCWSTR title; //タイトルバーの文字
#else
LPCSTR className; //クラス名
LPCSTR title; //タイトルバーの文字
#endif // UNICODE

bool windowInit;
UINT MessageCount;
protected:
RECT wrc;

	/*メンバ関数*/
public:
	Win32API();
#ifdef UNICODE
	Win32API(RECT windowSize, LPCWSTR title, LPCWSTR className = L"Windows");
#else
	Win32API(RECT windowSize, LPCSTR title, LPCSTR className = "Windows");
#endif // UNICODE
	~Win32API() {};

	BOOL WINAPI WindowInit();
	int WindowMessage();
	BOOL WINAPI WindowEnd();

};
