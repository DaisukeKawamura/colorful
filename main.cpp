#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
#include "./Header/LoadCSV.h"
#include "./Header/BlockChange.h"
/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"title"; //タイトル

const float clearColor[] = { 0.1f,0.1f,0.3f,0.0f }; //背景色

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
	hp.Init(25, 1, 60);

	// 画像の読み込み
	int background = draw.LoadTextrue(L"./Resources/background.png");
	int tex1 = draw.LoadTextrue(L"./Resources/tex1.png");
	int ringGraph = draw.LoadTextrue(L"./Resources/ring.png");
	int goalGraph = draw.LoadTextrue(L"./Resources/check.png");
	int boxGraph = draw.LoadTextrue(L"./Resources/box.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //カメラの位置
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //カメラの注視点
	XMFLOAT3 upVec = { 0, 1, 0 };        //上ベクトル

	// メインカメラの初期化
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// オブジェクトの生成
	int box = draw.Create3Dbox(20.0f, 20.0f, 20.0f);       //ブロック
	int boxFloor = draw.Create3Dbox(20.0f, 5.0f, 20.0f);   //床
	int startBox = draw.Create3Dbox(240.0f, 20.0f, 20.0f); //スタート部分の床
	int goalBox = draw.Create3Dbox(280.0f, 20.0f, 20.0f);  //ゴール部分の床
	int ringPolygon = draw.CreateCircle(10.0f, 16);        //リング
	int colorBox = draw.Create3Dbox(5.0f, 20.0f, 20.0f);   //色が一致していれば破壊出来るブロック
	int goalFlag = draw.CreateRect(100.0f, 20.0f);         //ゴールの旗

	// モデルの読み込み
	int itemModel = draw.CreateOBJModel("./Resources/item/4.obj", "./Resources/item/"); //アイテム

	// テクスチャのタイリング
	draw.NormalizeUV(startBox, boxGraph);
	draw.NormalizeUV(box, boxGraph);
	draw.NormalizeUV(boxFloor, boxGraph);
	draw.NormalizeUV(goalBox, boxGraph);

	// ゲームループで使う変数の宣言
	int map[MAP_HEIGHT][MAP_WIDTH] = {};                //CSVファイルの保存場所
	const float blockSize = 20.0f;                      //ステージをマップチップとして扱うための定数
	const XMFLOAT3 mapOffset = { 130.0f, 50.0f, 0.0f }; //ステージの描画開始位置（左上、オブジェクト）

	const float floorOffsetY = 10.0f;					//床のY軸の位置

	int mapColor[MAP_HEIGHT][MAP_WIDTH] = {};			//変わる色の情報を保持

	int ringCount = 0; //色変えリングの数
	int colorWallCount = 0; //色付き壁の数
	// 色変えリング等で使う色
	const XMFLOAT4 changeColor[] = {
		{ 0.964f, 0.176f, 0.352f, 1.0f },
		{ 0.9f, 0.9f, 0.0f, 1.0f }
	};

	size_t goalMapWidth = 90;

	const XMFLOAT3 startPlayerPos = { 130, -25, 0 }; //プレイヤーのスタート位置

	int laps = 1; //周回数

	bool isClear = false;    //クリアかどうか
	bool isGameover = false; //ゲームオーバーかどうか

	bool isHit = false;
	bool isLoopEnd = false; //無限ループを抜けるかどうか

	//int score = 0;

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
			hp.Init(25, 1, 60);

			player.pos = startPlayerPos;
			player.cameraPosX = player.pos.x;

			{
				int temp = LoadCSV(map, "./Resources/stage/stage1.csv");

				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}
			}

			// メインカメラの初期化
			draw.SetCamera(
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z - 170.0f),
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z),
				upVec, MAIN_CAMERA
			);

			laps = 1;
			//score = 0;
			isClear = false;
			isGameover = false;
			break;
		case GameStatus::Main:
			//score = 0;
			if (Input::IsKeyTrigger(DIK_SPACE))
			{
				if (isClear == true || isGameover == true)
				{
					gameStatus = GameStatus::Title;
				}
				else
				{
					player.jumpFlag = true;
				}
			}
			else
			{
				player.jumpFlag = false;
			}

			player.Update();

			// メインカメラの更新
			if (player.cameraPosX < goalMapWidth * blockSize + mapOffset.x)
			{
				draw.SetCamera(
					XMFLOAT3(player.cameraPosX + 100.0f, 30.0f, player.pos.z - 170.0f),
					XMFLOAT3(player.cameraPosX + 100.0f, 50.0f, player.pos.z),
					upVec, MAIN_CAMERA);
			}

			{
				OBB startOBB, goalOBB;

				startOBB.Initilize(XMFLOAT3(0.0f, -50.0f, 0.0f), XMMatrixIdentity(), 120.0f, 10.0f, 10.0f);
				goalOBB.Initilize(XMFLOAT3(goalMapWidth * blockSize + mapOffset.x * 2.0f, -50.0f, 0.0f), XMMatrixIdentity(), 140.0f, 10.0f, 10.0f);

				isHit = OBBCollision::ColOBBs(player.collision, startOBB);
				if (isHit)
				{//押し戻し処理
					OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, startOBB, isHit);
					player.groundFlag = true;
				}
				isHit = OBBCollision::ColOBBs(player.collision, goalOBB);
				if (isHit)
				{//押し戻し処理
					OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, goalOBB, isHit);
					player.groundFlag = true;
				}
			}

			ringCount = 0;
			colorWallCount = 0;

			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (size_t x = 0; x < goalMapWidth + 1; x++)
				{
					switch (map[y][x])
					{
					case ObjectStatus::BLOCK:
					{
						OBB blockOBB;

						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 10.0f, 10.0f);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						bool isHitDown = false;

						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
						}
						//地面についた時の処理
						if (isHitDown)
						{
							hp.AddDamage(0.01f);
							player.groundFlag = true;
							//変わる色の情報を保持
							mapColor[y][x] = BlockChange::changeBlockPColor(player.color);
						}
						else
						{
							//地面から離れたら保持していた色に変える
							if (mapColor[y][x] > 0)
							{
								map[y][x] = mapColor[y][x];

								mapColor[y][x] = 0;
							}
						}
					}
					break;
					case ObjectStatus::RedBLOCK:
					case ObjectStatus::BlueBLOCK:
					case ObjectStatus::GreenBLOCK:
					case ObjectStatus::YellowBLOCK:
					{
						//score++;
						OBB  blockOBB;
						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 10.0f, 10.0f);
						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						bool isHitDown = false;
						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
						}
						//地面についた時の処理
						if (isHitDown)
						{
							player.ChangeGroundColor(map[y][x]);
							hp.AddDamage(0.01f);
							player.groundFlag = true;
							//変わる色の情報を保持
							mapColor[y][x] = BlockChange::changeBlockPColor(player.color);
						}
						else
						{
							//地面から離れたら保持していた色に変える
							if (mapColor[y][x] > 0)
							{
								map[y][x] = mapColor[y][x];
								mapColor[y][x] = 0;
							}
						}
					}
					break;
					case ObjectStatus::Floor:
					{
						OBB  blockOBB;
						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y + floorOffsetY, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);
						bool HitDown = false;
						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
						}
						//地面に触れたらの処理
						if (HitDown)
						{
							player.groundFlag = true;
							//変わる色を保持
							mapColor[y][x] = BlockChange::changeFloorPColor(player.color);

							hp.AddDamage(0.005);
						}
						else
						{
							//プレイヤーの色に合わせてブロックを変える
							if (mapColor[y][x] > 0)
							{
								map[y][x] = mapColor[y][x];
								mapColor[y][x] = 0;
							}
						}
					}
					break;
					case ObjectStatus::RedFloor:
					case ObjectStatus::BlueFloor:
					case ObjectStatus::GreenFloor:
					case ObjectStatus::YellowFloor:
					{
						//score++;
						OBB  blockOBB;

						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y + floorOffsetY, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);
						bool HitDown = false;
						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
						}
						//地面に触れたらの処理
						if (HitDown)
						{
							player.groundFlag = true;
							player.ChangeGroundColor(map[y][x]);
							//変わる色を保持
							mapColor[y][x] = BlockChange::changeFloorPColor(player.color);

							hp.AddDamage(0.005);
						}
						else
						{
							//プレイヤーの色に合わせてブロックを変える
							if (mapColor[y][x] > 0)
							{
								map[y][x] = mapColor[y][x];
								mapColor[y][x] = 0;
							}
						}
					}
					break;
					case ObjectStatus::ITEM:
					{
						OBB itemOBB;
						itemOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 5.0f, 5.0f, 5.0f);
						bool isHit = OBBCollision::ColOBBs(player.collision, itemOBB);
						if (isHit)
						{
							hp.RecoverDamage(5);
							map[y][x] = ObjectStatus::BREAK_ITEM;
						}
					}
					break;
					case ObjectStatus::RING:
					{
						OBB blockOBB;

						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{
							player.color = changeColor[ringCount % (sizeof(changeColor) / sizeof(changeColor[0]))];
							map[y][x] = ObjectStatus::BREAK_RING;
							hp.RecoverDamage(5);
						}
						ringCount++;
					}
					break;
					case ObjectStatus::BREAK_RING:
						ringCount++;
						break;
					default:
						break;
					}
				}
			}

			if (player.pos.x > goalMapWidth * blockSize + mapOffset.x)
			{
				if (laps >= 2)
				{
					if (isGameover == false)
					{
						isClear = true;
					}
				}
				else
				{
					player.pos.x = mapOffset.x;
					player.cameraPosX = player.pos.x;

					laps++;
				}
			}
			if (player.pos.y <= -70.0f || player.cameraPosX - player.pos.x > 90)
			{
				if (isClear == false)
				{
					isGameover = true;
				}
			}
			if (isClear == false && isGameover == false)
			{
				isGameover = hp.isEmpty();
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

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);
		DirectDrawing::isDepthWriteBan = true;

		/* 描画 */

		// 背景
		//draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		switch (gameStatus)
		{
		case GameStatus::Title:
			break;
		case GameStatus::GameInit:
		case GameStatus::Main:
			ringCount = 0;
			colorWallCount = 0;
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (size_t x = 0; x < goalMapWidth + 1; x++)
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
							boxGraph
						);
						break;
					case ObjectStatus::Floor:
						draw.Draw(
							boxFloor,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y + floorOffsetY,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							boxGraph
						);
						break;
					case ObjectStatus::ITEM:
						draw.DrawOBJ(
							itemModel,
							XMFLOAT3(x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z),
							XMMatrixRotationX(XMConvertToRadians(0)),
							XMFLOAT3(5.0f, 5.0f, 5.0f),
							XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
						);
						break;
					case ObjectStatus::RING:
						draw.Draw(
							ringPolygon,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixRotationY(XMConvertToRadians(90)),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[ringCount % (sizeof(changeColor) / sizeof(changeColor[0]))],
							ringGraph
						);
						draw.Draw(
							ringPolygon,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixRotationY(XMConvertToRadians(-90)),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[ringCount % (sizeof(changeColor) / sizeof(changeColor[0]))],
							ringGraph
						);
						ringCount++;
						break;
					case ObjectStatus::BREAK_RING:
						ringCount++;
						break;
					case ObjectStatus::RedBLOCK:
						draw.Draw(
							box,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[0],
							boxGraph
						);
						break;
					case ObjectStatus::BlueBLOCK:
						break;
					case ObjectStatus::GreenBLOCK:
						break;
					case ObjectStatus::YellowBLOCK:
						draw.Draw(
							box,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[1],
							boxGraph
						);
						break;
					case ObjectStatus::RedFloor:
						draw.Draw(
							boxFloor,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y + floorOffsetY,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[0],
							boxGraph
						);
						break;
					case ObjectStatus::BlueFloor:
						break;
					case ObjectStatus::GreenFloor:
						break;
					case ObjectStatus::YellowFloor:
						draw.Draw(
							boxFloor,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y + floorOffsetY,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[1],
							boxGraph
						);
						break;
					default:
						break;
					}
				}
			}
			/*スタート地点*/
			draw.Draw(
				startBox,
				XMFLOAT3(0.0f, -50.0f, 0.0f),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				boxGraph
			);
			/*ゴール地点*/
			draw.Draw(
				goalFlag,
				XMFLOAT3(goalMapWidth * blockSize + mapOffset.x, 50.0f, 0.0f),
				XMMatrixRotationY(XMConvertToRadians(90)),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				goalGraph
			);
			draw.Draw(
				goalFlag,
				XMFLOAT3(goalMapWidth * blockSize + mapOffset.x, 50.0f, 0.0f),
				XMMatrixRotationY(XMConvertToRadians(-90)),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				goalGraph
			);
			draw.Draw(
				goalBox,
				XMFLOAT3(goalMapWidth * blockSize + mapOffset.x * 2.0f, -50.0f, 0),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				boxGraph
			);

			player.Draw();


#if _DEBUG
			if (isClear == true)
			{
				draw.DrawString(0, 0, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Clear");
			}
			if (isGameover == true)
			{
				draw.DrawString(window_width / 2 - 120, window_height / 2 - 160, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "GameOver");
			}
			draw.DrawString(0, 64, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "ink:%d", hp.GetCurrentHP());
			draw.DrawString(0, 0, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "laps:%d", laps);
			//draw.DrawString(0, 128, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "score:%d", score);
#endif // _DEBUG
			break;
		case GameStatus::Config:
			break;
		default:
			break;
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