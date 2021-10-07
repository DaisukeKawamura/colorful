#pragma once
#include <Windows.h>

class Win32API
{
public: // メンバ関数
	// デフォルトコンストラクタ
	Win32API();
#ifdef UNICODE
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="windowSize"> ウィンドウサイズ </param>
	/// <param name="title"> タイトル </param>
	/// <param name="className"> クラス名 </param>
	Win32API(RECT windowSize, LPCWSTR title, LPCWSTR className = L"Windows");
#else
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="windowSize"> ウィンドウサイズ </param>
	/// <param name="title"> タイトル </param>
	/// <param name="className"> クラス名 </param>
	Win32API(RECT windowSize, LPCSTR title, LPCSTR className = "Windows");
#endif // UNICODE
	// デストラクタ
	~Win32API() {};

	// 初期化
	BOOL WINAPI WindowInit();
	// ウィンドウメッセージ
	int WindowMessage();
	// 終了処理
	BOOL WINAPI WindowEnd();

public: // メンバ変数
WNDCLASSEX w;
HWND hWnd{};
#ifdef UNICODE
LPCWSTR className; //クラス名
LPCWSTR title; //タイトルバーの文字
#else
LPCSTR className; //クラス名
LPCSTR title; //タイトルバーの文字
#endif // UNICODE

bool windowInit; //初期化フラグ
UINT MessageCount; //WindowMessage()を通った回数
protected:
RECT wrc; //ウィンドウサイズ

};
