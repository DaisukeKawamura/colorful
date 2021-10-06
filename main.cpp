#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/DirectInput.h"
#include "./Header/SafeDelete.h"

/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"Fighter"; //タイトル

const float clearColor[] = { 0.1f,0.1f,0.1f,0.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	DirectXInit w{};

	w.title = title;
	for (UINT i = 0; i < 4; i++)
	{
		w.clearColor[i] = clearColor[i];
	}
	w.SetWindowSize(window_width, window_height);
	w.Init();

	DrawPolygon draw(&w);

	DirectInput* input = DirectInput::GetInstance();
	input->InputInit(w.w.hInstance, w.hWnd);

	char keys[256] = {};
	char oldkeys[256] = {};

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		for (size_t i = 0; i < 256; i++)
		{
			oldkeys[i] = keys[i];
		}
		input->GetHitKeyStateAll(keys);

		/* 更新 */

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);

		/* 描画 */

		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (input->CheckHitKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}