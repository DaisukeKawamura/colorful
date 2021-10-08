#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/DirectInput.h"
#include "./Header/SafeDelete.h"

/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"title"; //タイトル

const float clearColor[] = { 0.1f,0.1f,0.1f,0.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	using namespace DirectX;

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

	// 画像の読み込み
	int background = draw.LoadTextrue(L"./Resources/background.png");
	int tex1 = draw.LoadTextrue(L"./Resources/tex1.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //カメラの位置
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //カメラの注視点
	XMFLOAT3 upVec = { 0, 1, 0 }; //上ベクトル

	// メインカメラの初期化
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// オブジェクトの生成
	int triangle = draw.CreateTriangle(
		XMFLOAT3(-10.0f, -10.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		XMFLOAT3(  0.0f, +10.0f, 0.0f), XMFLOAT2(0.5f, 0.0f),
		XMFLOAT3(+10.0f, -10.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		true);
	int cylinder = draw.CreateCylinder(10.0f, 10.0f, 8, true);

	// ゲームループで使う変数の宣言
	XMFLOAT3 cylinderPos = { 0, 0, 0 }; //位置

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

		if (keys[DIK_LEFT])
		{
			cylinderPos.x -= 1.0f;
		}
		if (keys[DIK_RIGHT])
		{
			cylinderPos.x += 1.0f;
		}
		if (keys[DIK_UP])
		{
			cylinderPos.y += 1.0f;
		}
		if (keys[DIK_DOWN])
		{
			cylinderPos.y -= 1.0f;
		}

		if (keys[DIK_A])
		{
			cameraPos.x -= 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_D])
		{
			cameraPos.x += 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_W])
		{
			cameraPos.y += 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_S])
		{
			cameraPos.y -= 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);

		/* 描画 */

		// 背景
		draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		// 3dオブジェクト
		draw.Draw(
			triangle,
			XMFLOAT3(10.0f, 10.0f, 10.0f),
			XMMatrixRotationY(XMConvertToRadians(75)),
			XMFLOAT3(1, 1, 1),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			tex1
		);
		draw.Draw(
			cylinder,
			cylinderPos,
			XMMatrixIdentity(),
			XMFLOAT3(1, 1, 1),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		);

		// 前景
		draw.DrawTextrue(
			window_width,
			window_height,
			100,
			100,
			0,
			tex1,
			XMFLOAT2(1.0f, 1.0f)
		);
		draw.DrawCutTextrue(
			0,
			window_height,
			100,
			100,
			XMFLOAT2(0, 320),
			XMFLOAT2(320, 480),
			0,
			background,
			XMFLOAT2(0.0f, 1.0f)
		);

		// ループの終了処理
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (input->CheckHitKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}