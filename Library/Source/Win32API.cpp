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

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ
	if (AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false) == false)
	{
		return false;
	}

	hWnd = CreateWindow(
		w.lpszClassName,      //クラス名
		title,                //タイトルバーの文字
		WS_OVERLAPPEDWINDOW,  //標準的なウィンドウスタイル
		CW_USEDEFAULT,        //表示X座標(OSに任せる)
		CW_USEDEFAULT,        //表示Y座標(OSに任せる)
		wrc.right - wrc.left, //ウィンドウ横幅
		wrc.bottom - wrc.top, //ウィンドウ縦幅
		nullptr,              //親ウィンドウハンドル
		nullptr,              //メニューハンドル
		w.hInstance,          //呼び出しアプリケーションハンドル
		nullptr               //オプション
	);

	// ウィンドウ表示
	return ShowWindow(hWnd, SW_SHOW);
}

int Win32API::WindowMessage()
{
	MSG msg{}; //メッセージ

	MessageCount++;

	// メッセージある?
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); //キー入力メッセージの処理
		DispatchMessage(&msg); //プロシージャにメッセージを送る
	}

	// ×ボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return -1;
	}

	return 0;
}

BOOL WINAPI Win32API::WindowEnd()
{
	//ウィンドウクラスを登録解除
	return UnregisterClass(w.lpszClassName, w.hInstance);
}