#include "./Header/Win32API.h"

LRESULT WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

Win32API::Win32API() :
	w{},
	wrc{},
	hWnd{},
	windowInit(false),
	MessageCount(0)
{
#ifdef UNICODE
	className = L"DirectX";
	title = L"DirectXGame";
#else
	className = "DirectX";
	title = "DirectXGame";
#endif // UNICODE
}

#ifdef UNICODE
Win32API::Win32API(RECT windowSize, LPCWSTR title, LPCWSTR className) :
	w{},
	wrc{ windowSize },
	hWnd{},
	className{ className },
	title{ title },
	windowInit(false),
	MessageCount(0)
{
}
#else
Win32API::Win32API(RECT windowSize, LPCSTR title, LPCSTR className) :
	w{},
	wrc{ windowSize },
	hWnd{},
	className{ className },
	title{ title },
	windowInit(false),
	MessageCount(0)
{
}
#endif // UNICODE

BOOL WINAPI Win32API::WindowInit()
{
	if (windowInit == true)
	{
		return TRUE;
	}
	else
	{
		windowInit = true;
	}

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = className;
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&w);
	// �E�B���h�E�T�C�Y
	if (AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false) == false)
	{
		return false;
	}

	hWnd = CreateWindow(
		w.lpszClassName,      //�N���X��
		title,                //�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,  //�W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,        //�\��X���W(OS�ɔC����)
		CW_USEDEFAULT,        //�\��Y���W(OS�ɔC����)
		wrc.right - wrc.left, //�E�B���h�E����
		wrc.bottom - wrc.top, //�E�B���h�E�c��
		nullptr,              //�e�E�B���h�E�n���h��
		nullptr,              //���j���[�n���h��
		w.hInstance,          //�Ăяo���A�v���P�[�V�����n���h��
		nullptr               //�I�v�V����
	);

	// �E�B���h�E�\��
	return ShowWindow(hWnd, SW_SHOW);
}

int Win32API::WindowMessage()
{
	MSG msg{}; //���b�Z�[�W

	MessageCount++;

	// ���b�Z�[�W����?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); //�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); //�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	// �~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (msg.message == WM_QUIT)
	{
		return -1;
	}

	return 0;
}

BOOL WINAPI Win32API::WindowEnd()
{
	//�E�B���h�E�N���X��o�^����
	return UnregisterClass(w.lpszClassName, w.hInstance);
}