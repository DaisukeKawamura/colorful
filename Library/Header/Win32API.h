#pragma once
#include <Windows.h>

class Win32API
{
	/*�����o�ϐ�*/
public:
WNDCLASSEX w;
HWND hWnd{};
#ifdef UNICODE
LPCWSTR className; //�N���X��
LPCWSTR title; //�^�C�g���o�[�̕���
#else
LPCSTR className; //�N���X��
LPCSTR title; //�^�C�g���o�[�̕���
#endif // UNICODE

bool windowInit;
UINT MessageCount;
protected:
RECT wrc;

	/*�����o�֐�*/
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
