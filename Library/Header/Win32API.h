#pragma once
#include <Windows.h>

class Win32API
{
public: // �����o�֐�
	// �f�t�H���g�R���X�g���N�^
	Win32API();
#ifdef UNICODE
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="windowSize"> �E�B���h�E�T�C�Y </param>
	/// <param name="title"> �^�C�g�� </param>
	/// <param name="className"> �N���X�� </param>
	Win32API(RECT windowSize, LPCWSTR title, LPCWSTR className = L"Windows");
#else
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="windowSize"> �E�B���h�E�T�C�Y </param>
	/// <param name="title"> �^�C�g�� </param>
	/// <param name="className"> �N���X�� </param>
	Win32API(RECT windowSize, LPCSTR title, LPCSTR className = "Windows");
#endif // UNICODE
	// �f�X�g���N�^
	~Win32API() {};

	// ������
	BOOL WINAPI WindowInit();
	// �E�B���h�E���b�Z�[�W
	int WindowMessage();
	// �I������
	BOOL WINAPI WindowEnd();

public: // �����o�ϐ�
WNDCLASSEX w;
HWND hWnd{};
#ifdef UNICODE
LPCWSTR className; //�N���X��
LPCWSTR title; //�^�C�g���o�[�̕���
#else
LPCSTR className; //�N���X��
LPCSTR title; //�^�C�g���o�[�̕���
#endif // UNICODE

bool windowInit; //�������t���O
UINT MessageCount; //WindowMessage()��ʂ�����
protected:
RECT wrc; //�E�B���h�E�T�C�Y

};
