#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
#include "./Header/LoadCSV.h"

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
	int box = draw.Create3Dbox(20.0f, 5.0f, 20.0f);
	int startBox = draw.Create3Dbox(240.0f, 5.0f, 20.0f);

	// ゲームループで使う変数の宣言
	int map[MAP_HEIGHT][MAP_WIDTH] = {};
	const float blockSize = 20.0f;
	const XMFLOAT3 mapOffset = { 130.0f, 50.0f, 0.0f };

	XMFLOAT3 boxPos = { 0, 0, 0 }; //位置
	float angle = 0.0f;

	bool isGameover = false; //ゲームオーバーかどうか
	bool isLoopEnd = false;  //無限ループを抜けるかどうか

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

			player.Init(&draw);

			{
				int temp = LoadCSV(map, "./Resources/stage/stage1.csv");

				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}
			}

			// メインカメラの初期化
			//draw.SetCamera(XMFLOAT3(15.0f, 0.0f, -110.0f), XMFLOAT3(), upVec, MAIN_CAMERA);

			boxPos.x = 130.0f;
			boxPos.y = -50.0f;

			isGameover = false;
			break;
		case GameStatus::Main:
			if (isGameover == true)
			{
				if (Input::IsKeyTrigger(DIK_SPACE))
				{
					gameStatus = GameStatus::Title;
				}
			}

			player.Update();

			// メインカメラの更新
			draw.SetCamera(
				XMFLOAT3(player.pos.x, 0.0f, player.pos.z - 100.0f),
				XMFLOAT3(player.pos.x, 0.0f, player.pos.z),
				upVec, MAIN_CAMERA);

			angle += 1.0f;
			player.rotaMat = XMMatrixIdentity();
			player.rotaMat *= XMMatrixRotationZ(Player::gravity);
			player.rotaMat *= XMMatrixRotationX(XMConvertToRadians(angle));

			player.color.x += 0.01f;
			if (player.color.x > 1.0f)
			{
				player.color.x = 0.0f;
			}

			if (player.pos.y <= -50.0f)
			{
				isGameover = true;
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
		}
		if (Input::IsKey(DIK_RIGHT))
		{
		}
		if (Input::IsKey(DIK_UP))
		{
		}
		if (Input::IsKey(DIK_DOWN))
		{
		}

		if (Input::IsKey(DIK_A))
		{
		}
		if (Input::IsKey(DIK_D))
		{
		}
		if (Input::IsKey(DIK_W))
		{
		}
		if (Input::IsKey(DIK_S))
		{
		}
		//OBBの当たり判定
		OBB obb;
		obb.Initilize(boxPos, XMMatrixIdentity(), 110, 10, 10);
		bool isHit = OBBCollision::ColOBBs(player.collision, obb);

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);

		/* 描画 */

		// 背景
		draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		switch (gameStatus)
		{
		case GameStatus::Title:
			break;
		case GameStatus::GameInit:
			break;
		case GameStatus::Main:
			draw.Draw(
				startBox,
				XMFLOAT3(0, -50.0f, 0),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			);
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (int x = 0; x < sizeof(map[0]) / sizeof(map[0][0]); x++)
				{
					switch (map[y][x])
					{
					case ObjectStatus::BLOCK:
						draw.Draw(
							box,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							0
						);
						break;
					case ObjectStatus::ITEM:
						//draw->Draw(x * mapSize + offsetX, y * mapSize + offsetY, graphHandleArray[map[y][x]], transFlag);
						break;
					case ObjectStatus::RING:
						//draw->Draw(x * mapSize + offsetX, y * mapSize + offsetY, graphHandleArray[map[y][x]], transFlag);
						break;
					case ObjectStatus::COLOR_WALL:
						//draw->Draw(x * mapSize + offsetX, y * mapSize + offsetY, graphHandleArray[map[y][x]], transFlag);
						break;
					default:
						break;
					}
				}
			}
			/*一層目*/
			//draw.Draw(
			//	box,
			//	boxPos,
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 20.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 40.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 60.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 80.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 100.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x - 120.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x - 120.0f, boxPos.y, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			/*二層目*/
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 60.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 80.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 100.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 120.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 140.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 160.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 180.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 200.0f, boxPos.y + 40.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			/*三層目*/
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 120.0f, boxPos.y + 70.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 140.0f, boxPos.y + 70.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 160.0f, boxPos.y + 70.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 180.0f, boxPos.y + 70.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);
			//draw.Draw(
			//	box,
			//	XMFLOAT3(boxPos.x + 200.0f, boxPos.y + 70.0f, boxPos.z),
			//	XMMatrixIdentity(),
			//	XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			//);

			player.Draw();

			if (isGameover == true)
			{
#if _DEBUG
				draw.DrawString(0, 0, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "GameOver");
#endif // _DEBUG
			}
			break;
		case GameStatus::Config:
			break;
		default:
			break;
		}

		if (isHit)
		{
			draw.DrawString(0, 16 * 2.5f * 2, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "hit");
		}

		// ループの終了処理
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (isLoopEnd == true) { break; }
		if (Input::IsKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}