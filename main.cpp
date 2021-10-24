#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
#include "./Header/LoadCSV.h"
#include "./Header/BlockChange.h"
#include"./Header/Directing.h"
/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"title"; //タイトル

const float clearColor[] = { 0.1f,0.1f,0.3f,0.0f }; //背景色

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	enum GameStatus
	{
		Title,      //タイトル
		Select,     //ステージセレクト
		Main,       //ゲームメイン
		TitleInit,  //Titleの初期化
		SelectInit, //ステージセレクト
		MainInit    //Mainの初期化
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

	Directing directing;
	directing.Init();
	directing.ParticleInit(&draw);

	// 画像の読み込み
	int background1 = draw.LoadTextrue(L"./Resources/bag1.png");
	int background2 = draw.LoadTextrue(L"./Resources/bag2.png");
	int ringGraph = draw.LoadTextrue(L"./Resources/ring.png");
	int goalGraph = draw.LoadTextrue(L"./Resources/check.png");
	int boxGraph = draw.LoadTextrue(L"./Resources/box.png");
	int test = draw.LoadTextrue(L"./Resources/tex1.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //カメラの位置
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //カメラの注視点
	XMFLOAT3 upVec = { 0, 1, 0 };        //上ベクトル

	// メインカメラの初期化
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// オブジェクトの生成
	const XMFLOAT3 blockSize = { 40.0f, 20.0f, 20.0f };                         //ブロック一個分の大きさ
	int box = draw.Create3Dbox(blockSize.x, blockSize.y, blockSize.z);          //ブロック
	int boxFloor = draw.Create3Dbox(blockSize.x, blockSize.y / 4, blockSize.z); //床
	int startBox = draw.Create3Dbox(240.0f, blockSize.y, blockSize.z); //スタート部分の床
	int goalBox = draw.Create3Dbox(320.0f, blockSize.y, blockSize.z);  //ゴール部分の床
	int ringPolygon = draw.CreateCircle(10.0f, 16); //リング
	int goalFlag = draw.CreateRect(100.0f, 20.0f);  //ゴールの旗

	// モデルの読み込み
	int itemModel = draw.CreateOBJModel("./Resources/item/4.obj", "./Resources/item/"); //アイテム

	// テクスチャのタイリング
	draw.NormalizeUV(startBox, boxGraph);
	draw.NormalizeUV(box, boxGraph);
	draw.NormalizeUV(boxFloor, boxGraph);
	draw.NormalizeUV(goalBox, boxGraph);

	// ゲームループで使う変数の宣言
	int map[MAP_HEIGHT][MAP_WIDTH] = {};                //CSVファイルの保存場所
	const float mapTop = -50.0f + 20.0f * (MAP_HEIGHT - 1);
	const XMFLOAT3 mapOffset = { 130.0f, mapTop, 0.0f }; //ステージの描画開始位置（左上、オブジェクト）

	const float floorOffsetY = 10.0f;					//床のY軸の位置

	int mapColor[MAP_HEIGHT][MAP_WIDTH] = {};			//変わる色の情報を保持

	int ringCount = 0; //色変えリングの数
	const auto& changeColor = BlockChange::changeColor;
	int ringColor[10] = {}; //リングの色

	size_t goalMapWidth = 90;

	const XMFLOAT3 startPlayerPos = { 0, -25, 0 }; //プレイヤーのスタート位置

	int laps = 1; //周回数

	const int maxStageCount = 5 + 1;      //ステージ数＋タイトル
	int stageNo = 0;                      //選択されているステージ
	bool isStageClear[maxStageCount - 1]; //各ステージのクリアフラグ

	for (size_t i = 0; i < sizeof(isStageClear); i++)
	{
#if _DEBUG
		isStageClear[i] = true; //デバッグ用にステージを全開放
#else
		isStageClear[i] = false;
#endif
	}

	float scrollX = 0;

	bool isClear = false;    //クリアかどうか
	bool isGameover = false; //ゲームオーバーかどうか

	UINT64 nowTime = 0; //現在の時間
	int keyCount = 0;   //キー入力され続けたの時間
	bool isHit = false;
	bool isLoopEnd = false; //無限ループを抜けるかどうか

	//int score = 0;

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		Input::Update();
		nowTime++;

		/* 更新 */

		switch (gameStatus)
		{
		case GameStatus::TitleInit:
			gameStatus = GameStatus::Title;

			// メインカメラの初期化
			draw.SetCamera(
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z - 170.0f),
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z),
				upVec, MAIN_CAMERA
			);

		case GameStatus::Title:
			if (Input::IsKeyTrigger(DIK_SPACE))
			{
				gameStatus = GameStatus::SelectInit;
			}
			break;
		case GameStatus::SelectInit:
			gameStatus = GameStatus::Select;

			// メインカメラの初期化
			draw.SetCamera(
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z - 170.0f),
				XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z),
				upVec, MAIN_CAMERA
			);

			stageNo = 0;

		case GameStatus::Select:
			if (Input::IsKey(DIK_LEFT) && Input::IsKey(DIK_RIGHT))
			{
				keyCount = 0;
			}
			else if (Input::IsKey(DIK_LEFT))
			{
				keyCount++;

				if (keyCount % 10 == (10 - 1) % 10)
				{
					stageNo--;

					if (stageNo < 0)
					{
						stageNo += maxStageCount;
					}
				}
			}
			else if (Input::IsKey(DIK_RIGHT))
			{
				keyCount++;

				if (keyCount % 10 == (10 + 1) % 10)
				{
					stageNo++;
					stageNo %= maxStageCount;
				}
			}
			else
			{
				keyCount = 0;
			}


			if (Input::IsKeyTrigger(DIK_SPACE))
			{
				if (stageNo == maxStageCount - 1)
				{
					gameStatus = GameStatus::TitleInit;
				}
				else
				{
					if (stageNo > 0)
					{
						if (isStageClear[stageNo - 1] == true)
						{
							gameStatus = GameStatus::MainInit;
						}
					}
					else
					{
						gameStatus = GameStatus::MainInit;
					}
				}
			}
			break;
		case GameStatus::MainInit:
			gameStatus = GameStatus::Main;

			player.Init(&draw);
			hp.Init(25, 1, 60);
			directing.Init();
			player.pos = startPlayerPos;
			player.cameraPosX = player.pos.x;

			{
				int temp = 0;
				char* filePath = nullptr;
				char* ringFilePath = nullptr;
				XMFLOAT4 startColor = {};

				switch (stageNo)
				{
				case 0:
					filePath = (char*)"./Resources/stage/stage1.csv";
					ringFilePath = (char*)"./Resources/stage/ringColor1.csv";
					goalMapWidth = 100;
					startColor = changeColor[BlockChange::ColorNo::YELLOW];
					break;
				case 1:
					filePath = (char*)"./Resources/stage/stage2.csv";
					ringFilePath = (char*)"./Resources/stage/ringColor2.csv";
					goalMapWidth = 110;
					startColor = changeColor[BlockChange::ColorNo::YELLOW];
					break;
				default:
					filePath = (char*)"./Resources/stage/stage0.csv";
					ringFilePath = (char*)"./Resources/stage/ringColor1.csv";
					goalMapWidth = 90;
					startColor = changeColor[BlockChange::ColorNo::YELLOW];
					break;
				}

				temp = LoadCSV(map, filePath);
				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}
				temp = LoadCSV1D(ringColor, ringFilePath);
				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}

				if (goalMapWidth >= MAP_WIDTH)
				{
					goalMapWidth = MAP_WIDTH - 1;
				}

				player.color = startColor;
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
			if (player.cameraPosX < goalMapWidth * blockSize.x + mapOffset.x)
			{
				draw.SetCamera(
					XMFLOAT3(player.cameraPosX + 100.0f + directing.shakeX, 30.0f + directing.shakeY, player.pos.z - 170.0f),
					XMFLOAT3(player.cameraPosX + 100.0f, 50.0f, player.pos.z),
					upVec, MAIN_CAMERA);
			}

			{
				OBB startOBB, goalOBB;

				startOBB.Initilize(XMFLOAT3(0.0f, -50.0f, 0.0f), XMMatrixIdentity(), 120.0f, blockSize.y / 2, blockSize.z / 2);
				goalOBB.Initilize(XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x * 2.0f, -50.0f, 0.0f), XMMatrixIdentity(), 160.0f, blockSize.y / 2, blockSize.z / 2);

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

			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (size_t x = 0; x < goalMapWidth + 1; x++)
				{
					XMFLOAT3 mapPosition = { x * blockSize.x + mapOffset.x, y * (-blockSize.y) + mapOffset.y, mapOffset.z };
					switch (map[y][x])
					{
					case ObjectStatus::BLOCK:
					{
						OBB blockOBB;

						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / 2, blockSize.z / 2);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						bool isHitDown = false;

						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown, map[y + 1][x], map[y - 1][x]);
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
						OBB blockOBB;
						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / 2, blockSize.z / 2);
						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						bool isHitDown = false;
						if (isHit)
						{//押し戻し処理
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown, map[y + 1][x], map[y - 1][x]);
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
						OBB blockOBB;
						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);
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

							hp.AddDamage(0.005f);
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
						OBB blockOBB;

						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);
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

							hp.AddDamage(0.005f);
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
						itemOBB.Initilize(mapPosition, XMMatrixIdentity(), 5.0f, 5.0f, 5.0f);
						bool isHit = OBBCollision::ColOBBs(player.collision, itemOBB);
						if (isHit)
						{
							directing.ItemStart(XMFLOAT3(x * blockSize.x + mapOffset.x, y * (-blockSize.y) + mapOffset.y, mapOffset.z),
								XMFLOAT3(player.cameraPosX - 35, 110, 0), 50, player.cameraPosX);
							hp.RecoverDamage(5);
							map[y][x] = ObjectStatus::BREAK_ITEM;
						}
					}
					break;
					case ObjectStatus::RING:
					{
						OBB blockOBB;

						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), 10.0f, 10.0f, 10.0f);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{
							directing.RingStart();//リングパーティクルスタート
							player.color = changeColor[ringColor[ringCount % (sizeof(ringColor) / sizeof(ringColor[0]))]];
							map[y][x] = ObjectStatus::BREAK_RING;
							hp.RecoverDamage(5);
						}
						ringCount++;
					}
					break;
					case ObjectStatus::BREAK_RING:
						ringCount++;
						break;
					case ObjectStatus::COLLECTION:
					{
						OBB blockOBB;

						blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{
							map[y][x] = ObjectStatus::NONE;
						}
					}
					default:
						break;
					}
				}
			}

			directing.RunUpdate(player.pos, player.color);
			directing.RingUpdate(player.pos, player.color);
			directing.ShakeUpdate();

			if (player.pos.x > goalMapWidth * blockSize.x + mapOffset.x)
			{
				if (laps >= 2)
				{
					if (isGameover == false)
					{
						isClear = true;
						isStageClear[stageNo] = true;
						directing.scoreEasingStart(XMFLOAT3(200, -400, 0), XMFLOAT3(200, 200, 0), 20);
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
					directing.ShakeStart(10.0f, 10);
				}
			}
			if (isClear == false && isGameover == false)
			{
				isGameover = hp.isEmpty();
			}
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

		/* 描画 */

		// 背景
		draw.DrawTextrue(scrollX, 0, window_width, window_height, 0, background1, XMFLOAT2(0.0f, 0.0f));
		draw.DrawTextrue(scrollX + window_width, 0, window_width, window_height, 0, background2, XMFLOAT2(0.0f, 0.0f));
		draw.DrawTextrue(scrollX + window_width * 2, 0, window_width, window_height, 0, background1, XMFLOAT2(0.0f, 0.0f));

		scrollX -= player.speed;

		if (scrollX <= -window_width * 2)
		{
			scrollX += window_width * 2;
		}

		switch (gameStatus)
		{
		case GameStatus::TitleInit:
		case GameStatus::Title:
			draw.SetDrawBlendMode(BLENDMODE_ALPHA);
			DirectDrawing::isDepthWriteBan = false;

			draw.DrawString(window_width / 2 - 120, window_height / 2 - 160, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "PAINTWO");
			draw.DrawString(window_width / 2 - 120, window_height / 2 + 100, 3.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Please SPACE");
			break;
		case GameStatus::SelectInit:
		case GameStatus::Select:
			if (stageNo == maxStageCount - 1)
			{
				draw.DrawString(window_width / 2 - 120, window_height / 2 - 40, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Title");
			}
			else
			{
				draw.DrawString(window_width / 2 - 20, window_height / 2 - 40, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "%d", stageNo + 1);
			}
			break;
		case GameStatus::MainInit:
		case GameStatus::Main:
			ringCount = 0;
			for (int y = 0; y < MAP_HEIGHT; y++)
			{
				for (size_t x = 0; x < goalMapWidth + 1; x++)
				{
					XMFLOAT3 mapPosition = { x * blockSize.x + mapOffset.x, y * (-blockSize.y) + mapOffset.y, mapOffset.z };
					switch (map[y][x])
					{
					case ObjectStatus::BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							boxGraph
						);
						break;
					case ObjectStatus::Floor:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							boxGraph
						);
						break;
					case ObjectStatus::ITEM:
						draw.DrawOBJ(
							itemModel,
							mapPosition,
							XMMatrixRotationX(XMConvertToRadians(0)),
							XMFLOAT3(5.0f, 5.0f, 5.0f),
							XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
						);
						break;
					case ObjectStatus::RING:
						draw.Draw(
							ringPolygon,
							mapPosition,
							XMMatrixRotationY(XMConvertToRadians(90)),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[ringColor[ringCount % (sizeof(ringColor) / sizeof(ringColor[0]))]],
							ringGraph
						);
						draw.Draw(
							ringPolygon,
							mapPosition,
							XMMatrixRotationY(XMConvertToRadians(-90)),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[ringColor[ringCount % (sizeof(ringColor) / sizeof(ringColor[0]))]],
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
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::RED],
							boxGraph
						);
						break;
					case ObjectStatus::BlueBLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::BLUE],
							boxGraph
						);
						break;
					case ObjectStatus::GreenBLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::GREEN],
							boxGraph
						);
						break;
					case ObjectStatus::YellowBLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::YELLOW],
							boxGraph
						);
						break;
					case ObjectStatus::RedFloor:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::RED],
							boxGraph
						);
						break;
					case ObjectStatus::BlueFloor:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::BLUE],
							boxGraph
						);
						break;
					case ObjectStatus::GreenFloor:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::GREEN],
							boxGraph
						);
						break;
					case ObjectStatus::YellowFloor:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::YELLOW],
							boxGraph
						);
						break;
					case ObjectStatus::COLLECTION:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixRotationX(XMConvertToRadians(90)),
							XMFLOAT3(1, 1, 1),
							XMFLOAT4(1, 1, 1, 1),
							test
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
				XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x, 50.0f, 0.0f),
				XMMatrixRotationY(XMConvertToRadians(90)),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				goalGraph
			);
			draw.Draw(
				goalFlag,
				XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x, 50.0f, 0.0f),
				XMMatrixRotationY(XMConvertToRadians(-90)),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				goalGraph
			);
			draw.Draw(
				goalBox,
				XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x * 2.0f, -50.0f, 0),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				boxGraph
			);

			player.Draw();

			//アイテムイージング
			if (directing.itemFlag == true)
			{
				XMFLOAT3 itemPos = directing.ItemUpdate(XMFLOAT3(player.cameraPosX, 0.0f, 0));

				draw.DrawOBJ(
					itemModel,
					itemPos,
					XMMatrixRotationX(XMConvertToRadians(0)),
					directing.itemScale,
					XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
				);
			}

			DirectDrawing::isDepthWriteBan = true;
			draw.SetDrawBlendMode(BLENDMODE_ADD);
			directing.RunDraw();
			directing.RingDraw();
			draw.SetDrawBlendMode(BLENDMODE_ALPHA);
			DirectDrawing::isDepthWriteBan = false;

			if (isClear == true)
			{
				XMFLOAT3 scorePos = directing.scoreEasing();
				draw.DrawString(scorePos.x, scorePos.y, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Clear");
			}
			if (isGameover == true)
			{
				draw.DrawString(window_width / 2 - 120, window_height / 2 - 160, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "GameOver");
			}
#if _DEBUG
			draw.DrawString(0, 0, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "laps:%d", laps);
			draw.DrawString(0, 64, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "ink:%d", hp.GetCurrentHP());
			draw.DrawString(0, 128, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "stage:%d", stageNo + 1);
			//draw.DrawString(0, 128, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "score:%d", score);
#endif // _DEBUG
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