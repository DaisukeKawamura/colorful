#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"title"; //タイトル

const float clearColor[] = { 0.1f,0.1f,0.1f,0.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	enum GameStatus
	{
		Title,    //タイトル
		GameInit, //Mainの初期化
		Main,     //ゲームメイン
		Config    //設定
	} gameStatus = GameStatus::Title;

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

	Input::Init(w);

	Player player;
	player.Init(&draw);

	HP hp;
	hp.Init(10, 1, 20);

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
		XMFLOAT3(+10.0f, -10.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)
	);
	int box = draw.Create3Dbox(10.0f, 10.0f, 10.0f);

	// ゲームループで使う変数の宣言
	XMFLOAT3 boxPos = { 0, 0, 0 }; //位置
	float angle = 0.0f;

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		Input::Update();

		/* 更新 */

		switch (gameStatus)
		{
		case GameStatus::Title:
			gameStatus = GameStatus::GameInit;
			break;
		case GameStatus::GameInit:
			gameStatus = GameStatus::Main;

			player.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case GameStatus::Main:
			//gameStatus = GameStatus::Title;

			player.Update();

			angle += 1.0f;
			player.rotaMat = XMMatrixIdentity();
			player.rotaMat *= XMMatrixRotationZ(Player::gravity);
			player.rotaMat *= XMMatrixRotationX(XMConvertToRadians(angle));

			player.color.x += 0.01f;
			if (player.color.x > 1.0f)
			{
				player.color.x = 0.0f;
			}
			break;
		case GameStatus::Config:
			gameStatus = GameStatus::Title;
			break;
		default:
			gameStatus = GameStatus::Title;
			break;
		}

		if (Input::IsKey(DIK_LEFT))
		{
			boxPos.x -= 1.0f;
		}
		if (Input::IsKey(DIK_RIGHT))
		{
			boxPos.x += 1.0f;
		}
		if (Input::IsKey(DIK_UP))
		{
			boxPos.y += 1.0f;
		}
		if (Input::IsKey(DIK_DOWN))
		{
			boxPos.y -= 1.0f;
		}

		if (Input::IsKey(DIK_A))
		{
			cameraPos.x -= 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (Input::IsKey(DIK_D))
		{
			cameraPos.x += 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (Input::IsKey(DIK_W))
		{
			cameraPos.y += 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (Input::IsKey(DIK_S))
		{
			cameraPos.y -= 1.0f;

			// メインカメラの更新
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		//OBBの当たり判定
		OBB obb1;
		obb1.Initilize(player.pos, player.rotaMat, 5, 5, 5);
		OBB obb2;
		obb2.Initilize(boxPos, XMMatrixIdentity(), 5, 5, 5);
		bool isHit = OBBCollision::ColOBBs(obb1, obb2);

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);

		/* 描画 */

		// 背景
		draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		switch (gameStatus)
		{
		case GameStatus::Title:
			gameStatus = GameStatus::GameInit;
			break;
		case GameStatus::GameInit:
			gameStatus = GameStatus::Main;
			break;
		case GameStatus::Main:
			//gameStatus = GameStatus::Title;
			player.Draw();
			break;
		case GameStatus::Config:
			gameStatus = GameStatus::Title;
			break;
		default:
			gameStatus = GameStatus::Title;
			break;
		}

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
			box,
			boxPos,
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

		draw.DrawString(0, 0, 2.5f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "cameraPos.x:%f", cameraPos.x);
		draw.DrawString(0, 16 * 2.5f, 2.5f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "cameraPos.y:%f", cameraPos.y);

		if (isHit)
		{
			draw.DrawString(0, 16 * 2.5f * 2, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "hit");
		}

		// ループの終了処理
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (Input::IsKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}