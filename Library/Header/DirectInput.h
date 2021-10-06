#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// CheckHitKeyAll で調べる入力タイプ
#define CHECKINPUT_KEY   (0x01) //キー入力を調べる
#define CHECKINPUT_PAD   (0x02) //パッド入力を調べる
#define CHECKINPUT_MOUSE (0x04) //マウスボタン入力を調べる
#define CHECKINPUT_ALL   (CHECKINPUT_KEY | CHECKINPUT_PAD | CHECKINPUT_MOUSE) //すべての入力を調べる

//namespace DirectInput
//{
//	int InputInit(HINSTANCE hInstance, HWND hWnd);
//	bool CheckHitKey(int keyCode);
//	int CheckHitKeyAll(int CheckType = CHECKINPUT_ALL);
//	int GetHitKeyStateAll(char* KeyStateArray);
//}

//using namespace DirectInput;

class DirectInput final
{
private:
	// コンストラクタをprivateにする
	DirectInput() {};
	// デストラクタをprivateにする
	~DirectInput() {};
public:
	// コピーコンストラクタを無効にする
	DirectInput(const DirectInput& obj) = delete;
	// 代入演算子を無効にする
	const DirectInput& operator=(const DirectInput& obj) = delete;
	// インスタンスの取得
	static DirectInput* GetInstance();

public: // 静的メンバ変数
	static BYTE key[256];
private:
	static IDirectInput8* dinput;
	static IDirectInputDevice8* devkeyborad;

public: //メンバ関数
	// 入力デバイスの初期化
	int InputInit(HINSTANCE hInstance, HWND hWnd);
	// 特定のキーボードの入力状態の取得
	bool CheckHitKey(int keyCode);
	// 全ての入力デバイスの状態の取得
	int CheckHitKeyAll(int CheckType = CHECKINPUT_ALL);
	// キーボードデバイスの入力状態の取得
	int GetHitKeyStateAll(char* KeyStateArray);
private:
	// キーボードデバイスの更新
	int KeyInputUpdate();

};
