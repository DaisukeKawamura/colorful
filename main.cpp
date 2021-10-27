#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/LoadCSV.h"
#include "./Header/BlockChange.h"
#include "./Header/Directing.h"
#include"./Header/Score.h"
#include "./Header/Warp.h"
#include "./Header/Audio.h"
/*ウィンドウサイズ*/
const int window_width = 1280; //横幅
const int window_height = 720; //縦幅

const WCHAR title[] = L"PAINTWO"; //タイトル

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
	} gameStatus = GameStatus::Title, oldGameStatus = gameStatus;

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

	Player player = {};
	player.Init(&draw);

	Directing directing = {};
	directing.Init();
	directing.ParticleInit(&draw);

	Score stageScore;
	stageScore.Init();

	Warp warp = {};
	warp.Init(&draw);

	Audio audio;
	audio.Init();

	// 画像の読み込み
	int background1 = draw.LoadTextrue(L"./Resources/bag1.png");
	int background2 = draw.LoadTextrue(L"./Resources/bag2.png");
	int ringGraph = draw.LoadTextrue(L"./Resources/ring.png");
	int goalGraph = draw.LoadTextrue(L"./Resources/check.png");
	int boxGraph = draw.LoadTextrue(L"./Resources/box.png");
	int clearGraph = draw.LoadTextrue(L"./Resources/stageclear.png");
	int gameoverGraph = draw.LoadTextrue(L"./Resources/gameover.png");
	int titleGraph = draw.LoadTextrue(L"./Resources/title.png");
	int testGraph = draw.LoadTextrue(L"./Resources/tex1.png"); //仮描画用の画像
	int breakwallGraph = draw.LoadTextrue(L"./Resources/goal1.png");
	int noPaintGraph = draw.LoadTextrue(L"./Resources/black.png");
	int playerScoreGraph = draw.LoadTextrue(L"./Resources/bag3.png");
	// オブジェクトの生成
	const XMFLOAT3 blockSize = { 40.0f, 20.0f, 20.0f };                         //ブロック一個分の大きさ
	int box = draw.Create3Dbox(blockSize.x, blockSize.y, blockSize.z);          //ブロック
	int boxFloor = draw.Create3Dbox(blockSize.x, blockSize.y / 4, blockSize.z); //床
	int startBox = draw.Create3Dbox(240.0f, blockSize.y, blockSize.z); //スタート部分の床
	int goalBox = draw.Create3Dbox(320.0f, blockSize.y, blockSize.z);  //ゴール部分の床
	int goalFlag = draw.CreateRect(100.0f, 20.0f);  //ゴールの旗
	int wallBreak = draw.Create3Dbox(20.0f, 20.0f, 20.0f);//壊れる壁
	int testPolygon = draw.CreateTriangle({ 0.0f, 1.0f, 0.0f }, { 0.5f, 0.0f }, { 1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }, { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }); //仮描画用のポリゴン

	// モデルの読み込み
	//int itemModel = draw.CreateOBJModel("./Resources/item/4.obj", "./Resources/item/");    //アイテム
	int ringModel = draw.CreateOBJModel("./Resources/ring/5.obj", "./Resources/ring/");    //リング
	int coinModel = draw.CreateOBJModel("./Resources/coin/coin.obj", "./Resources/coin/"); //収集アイテムのコイン

	// テクスチャのタイリング
	draw.NormalizeUV(startBox, boxGraph);
	draw.NormalizeUV(box, boxGraph);
	draw.NormalizeUV(boxFloor, boxGraph);
	draw.NormalizeUV(goalBox, boxGraph);

	// 音声データの読み込み
	int testSound = audio.SoundLoadWave("./Resources/Sound/Alarm01.wav");
	audio.SoundPlayWave(testSound);

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //カメラの位置
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //カメラの注視点
	XMFLOAT3 upVec = { 0, 1, 0 };        //上ベクトル

	// メインカメラの初期化
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// ゲームループで使う変数の宣言
	int map[MAP_HEIGHT][MAP_WIDTH] = {};                //CSVファイルの保存場所
	const float mapTop = -50.0f + 20.0f * (MAP_HEIGHT - 1);
	const XMFLOAT3 mapOffset = { 130.0f, mapTop, 0.0f }; //ステージの描画開始位置（左上、オブジェクト）

	const float floorOffsetY = 10.0f;					//床のY軸の位置

	int ringCount = 0; //色変えリングの数
	const auto &changeColor = BlockChange::changeColor;
	int ringColor[10] = {}; //リングの色

	int warpCount = 0;

	size_t goalMapWidth = 90;

	const XMFLOAT3 startPlayerPos = { 0, -25, 0 }; //プレイヤーのスタート位置

	int laps = 1; //周回数

	const int maxStageCount = 6;      //ステージ数
	int stageNo = 0;                      //選択されているステージ
	bool stageSelectTitle = false;       //ステージ選択でタイトルを選択してるかどうか
	bool isStageClear[maxStageCount]; //各ステージのクリアフラグ
	float blockCount = 0.0f;      //塗れるブロックの総数
	float paintBlockCount = 0.0f; //塗ったブロックの数

	for (size_t i = 0; i < sizeof(isStageClear); i++)
	{
		isStageClear[i] = true; //デバッグ用にステージを全開放
	}
	//リトライがtrueゲームオーバーはfalse
	bool selectRetryFlag = true;
	int score = 0; //スコア
	int medal = 0;//メダル数

	bool isClear = false;    //クリアかどうか
	bool isGameover = false; //ゲームオーバーかどうか

	float scrollX = 0; //背景のスクロール値

	UINT64 nowTime = 0; //現在の時間
	int keyCount = 0;   //キー入力され続けたの時間
	bool isHit = false;
	bool isLoopEnd = false; //無限ループを抜けるかどうか

	float colorDrawHeight = 0;//塗った量の画像の大きさ
	float colorDrawOne = 0.93f;//1%当たりの塗る量

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		Input::Update();
		nowTime++;

		oldGameStatus = gameStatus;

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
				directing.StartSceneChange();
				//gameStatus = GameStatus::SelectInit;
			}
			if (directing.ChangeScene())
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

			directing.stageSelectInit();
		case GameStatus::Select:
			if (Input::IsKey(DIK_LEFT) && Input::IsKey(DIK_RIGHT))
			{
				keyCount = 0;
			}
			else if (Input::IsKey(DIK_LEFT))
			{
				keyCount++;
				stageSelectTitle = false;
				if (keyCount % 10 == (10 + 1) % 10)
				{
					if (directing.stageSelectFlag == true)
					{
						stageNo--;
						directing.stageSelectEasingStart(XMFLOAT3((float)(-window_width * (stageNo + 1)), 0, 0),
							XMFLOAT3((float)(-window_width * stageNo), 0, 0), 40.0f);

						if (stageNo < 0)
						{
							stageNo += maxStageCount;
						}
					}
				}
			}
			else if (Input::IsKey(DIK_RIGHT))
			{
				keyCount++;
				stageSelectTitle = false;
				if (keyCount % 10 == (10 + 1) % 10)
				{
					if (directing.stageSelectFlag == true)
					{
						directing.stageSelectEasingStart(XMFLOAT3((float)(-window_width * stageNo), 0, 0),
							XMFLOAT3((float)(-window_width * (stageNo + 1)), 0, 0), 40.0f);

						stageNo++;
						if (stageNo == maxStageCount)
						{
							stageNo = 0;
						}
					}
				}
			}
			else
			{
				keyCount = 0;
			}

			if (Input::IsKey(DIK_UP))
			{
				stageSelectTitle = true;
			}
			else if (Input::IsKey(DIK_DOWN))
			{
				stageSelectTitle = false;
			}

			if (Input::IsKeyTrigger(DIK_SPACE))
			{
				directing.StartSceneChange();
			}
			if (directing.ChangeScene())
			{
				if (stageSelectTitle == true)
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
			directing.StageSelectUpdate();
			break;
		case GameStatus::MainInit:
			gameStatus = GameStatus::Main;

			player.Init(&draw);
			directing.Init();
			player.pos = startPlayerPos;
			player.cameraPosX = player.pos.x;

			{
				int temp = 0;
				char *filePath = nullptr;
				char *ringFilePath = nullptr;
				XMFLOAT4 startColor = {};

				switch (stageNo)
				{
				case 0:
					filePath = (char *)"./Resources/stage/stage1.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor1.csv";
					goalMapWidth = 50;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				case 1:
					filePath = (char *)"./Resources/stage/stage2.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor3.csv";
					goalMapWidth = 50;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				case 2://
					filePath = (char *)"./Resources/stage/stage3.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor1.csv";
					goalMapWidth = 60;
					startColor = changeColor[BlockChange::ColorNo::BLUE];
					break;
				case 3://
					filePath = (char *)"./Resources/stage/stage4.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor4.csv";
					goalMapWidth = 60;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				case 4:
					filePath = (char *)"./Resources/stage/stage5.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor5.csv";
					goalMapWidth = 60;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				case 5:
					filePath = (char *)"./Resources/stage/stage6.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor6.csv";
					goalMapWidth = 50;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				default:
					filePath = (char *)"./Resources/stage/stage1.csv";
					ringFilePath = (char *)"./Resources/stage/ringColor1.csv";
					goalMapWidth = 90;
					startColor = changeColor[BlockChange::ColorNo::GREEN];
					break;
				}

				temp = LoadCSV(map, filePath);
				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}
				temp = LoadCSV1D(ringColor, sizeof(ringColor) / sizeof(ringColor[0]), ringFilePath);
				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}

				warp.WarpInit(map, stageNo);

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

			blockCount = 0.0f;
			paintBlockCount = 0.0f;
			for (size_t y = 0; y < MAP_HEIGHT; y++)
			{
				for (size_t x = 0; x < goalMapWidth; x++)
				{
					if (map[y][x] == ObjectStatus::BLOCK || map[y][x] == ObjectStatus::FLOOR)
					{
						blockCount += 1.0f;
					}
				}
			}
			colorDrawHeight = 0;
			laps = 1;
			score = 0;
			medal = 0;
			isClear = false;
			isGameover = false;
		case GameStatus::Main:
			if (Input::IsKeyTrigger(DIK_SPACE))
			{
				if (isClear == true || isGameover == true)
				{
					directing.StartSceneChange();
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

			if ((isClear == true || isGameover == true) && (Input::IsKeyTrigger(DIK_LEFT) || Input::IsKeyTrigger(DIK_RIGHT)))
			{//タイトルかリトライか
				if (selectRetryFlag == true)
				{
					selectRetryFlag = false;
				}
				else
				{
					selectRetryFlag = true;
				}
			}


			if (directing.ChangeScene() && (isClear == true || isGameover == true))
			{
				if (selectRetryFlag == true)
				{
					gameStatus = GameStatus::MainInit;
				}
				else
				{
					gameStatus = GameStatus::Select;
				}
			}

			player.Update();

			// メインカメラの更新
			if (player.cameraPosX < goalMapWidth * blockSize.x + mapOffset.x)
			{
				draw.SetCamera(
					XMFLOAT3(player.cameraPosX + 100.0f + directing.shakeX, 30.0f + directing.shakeY, player.pos.z - 260.0f),
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
			warpCount = 0;
			if (directing.pFlyFlag == true)
			{
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

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool isHitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown, map[(y + 1) % MAP_HEIGHT][x], map[(y - 1) % MAP_HEIGHT][x]);
							}
							//地面についた時の処理
							if (isHitDown)
							{
								player.groundFlag = true;
								//地面の色を変える
								map[y][x] = BlockChange::changeBlockPColor(player.color);

								paintBlockCount += 1.0f;
							}

						}
						break;
						case ObjectStatus::NOPAINT_BLOCK:
						{
							OBB blockOBB;

							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / 2, blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool isHitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown, map[(y + 1) % MAP_HEIGHT][x], map[(y - 1) % MAP_HEIGHT][x]);
							}
							//地面についた時の処理
							if (isHitDown)
							{
								//hp.AddDamage(0.01f);
								player.groundFlag = true;
							}
						}
						break;
						case ObjectStatus::RED_BLOCK:
						case ObjectStatus::BLUE_BLOCK:
						case ObjectStatus::GREEN_BLOCK:
						case ObjectStatus::YELLOW_BLOCK:
						{
							OBB blockOBB;
							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / 2, blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool isHitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown, map[(y + 1) % MAP_HEIGHT][x], map[(y - 1) % MAP_HEIGHT][x]);
							}
							//地面についた時の処理
							if (isHitDown)
							{
								player.ChangeGroundColor(map[y][x]);
								player.groundFlag = true;
								//地面の色を変える
								map[y][x] = BlockChange::changeBlockPColor(player.color);
							}

						}
						break;
						case ObjectStatus::FLOOR:
						{
							OBB blockOBB;
							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool HitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
							}
							//地面に触れたらの処理
							if (HitDown)
							{
								player.groundFlag = true;
								//地面の色を変える
								map[y][x] = BlockChange::changeFloorPColor(player.color);

								paintBlockCount += 1.0f;
							}

						}
						break;
						case ObjectStatus::NOPAINT_FLOOR:
						{
							OBB blockOBB;
							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool isHitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
							}
							//地面についた時の処理
							if (isHitDown)
							{
								//hp.AddDamage(0.005f);
								player.groundFlag = true;
							}
						}
						break;
						case ObjectStatus::RED_FLOOR:
						case ObjectStatus::BLUE_FLOOR:
						case ObjectStatus::GREEN_FLOOR:
						case ObjectStatus::YELLOW_FLOOR:
						{
							OBB blockOBB;
							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / 2, blockSize.y / (2 * 4), blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							bool HitDown = false;

							if (isHit)
							{//押し戻し処理
								OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
							}
							//地面に触れたらの処理
							if (HitDown)
							{
								player.groundFlag = true;
								player.ChangeGroundColor(map[y][x]);
								//地面の色を変える
								map[y][x] = BlockChange::changeFloorPColor(player.color);
							}
						}
						break;
						/*case ObjectStatus::ITEM:
						{
							OBB itemOBB;
							itemOBB.Initilize(mapPosition, XMMatrixIdentity(), 5.0f, 5.0f, 5.0f);
							bool isHit = OBBCollision::ColOBBs(player.collision, itemOBB);
							if (isHit)
							{
								directing.ItemStart(XMFLOAT3(x * blockSize.x + mapOffset.x, y * (-blockSize.y) + mapOffset.y, mapOffset.z),
									XMFLOAT3(player.cameraPosX - 35, 110, 0), 50, player.cameraPosX);
								map[y][x] = ObjectStatus::BREAK_ITEM;
							}
						}
						break;*/
						case ObjectStatus::RING:
						{
							OBB blockOBB;

							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), 5.0f, 10.0f, 10.0f);

							bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							if (isHit)
							{
								directing.RingStart();//リングパーティクルスタート
								player.color = changeColor[ringColor[ringCount % (sizeof(ringColor) / sizeof(ringColor[0]))]];
								map[y][x] = ObjectStatus::BREAK_RING;
							}
						}
						case ObjectStatus::BREAK_RING:
							ringCount++;
							break;
						case ObjectStatus::WARP:
						{
							OBB blockOBB;

							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / (2 * 4), blockSize.y / 2, blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							if (isHit)
							{
								auto temp = warp.WarpPos(warpCount);

								if ((temp.x != -1.0f) || (temp.y != -1.0f))
								{
									XMFLOAT3 warpPosition = {
										temp.x * blockSize.x + mapOffset.x,
										temp.y * (-blockSize.y) + mapOffset.y,
										mapOffset.z
									};
									player.pos = warpPosition;
									player.collision.Initilize(player.pos, player.rotaMat, 5, 5, 5);
								}
							}
							warp.ParticleUpdate(warpCount);
							warpCount++;
						}
						break;
						case ObjectStatus::COLLECTION:
						{
							OBB blockOBB;

							blockOBB.Initilize(mapPosition, XMMatrixIdentity(), blockSize.x / (2 * 4), blockSize.y / 2, blockSize.z / 2);

							isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
							if (isHit)
							{
								medal++;
								// 仮の演出
								directing.RingStart(); //リングパーティクルスタート
								map[y][x] = ObjectStatus::NONE;
							}
						}
						break;
						default:
							break;
						}
					}
				}
			}
			directing.RunUpdate(player.pos, player.color);
			directing.RingUpdate(player.pos, player.color);
			directing.ShakeUpdate();
			directing.DoubleJumpUpdate(player.pos);
			directing.StageSelectUpdate();
			if (directing.JumEfectJudge(player.jumpCount, player.color) && Input::IsKeyTrigger(DIK_SPACE))
			{
				directing.JumEfectStart();
			}
			if (directing.wallFlag == false)
			{
				directing.BreakWall(XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x, -50.0f, 0), player.pos);
			}
			if (player.pos.x > (goalMapWidth - 1) * blockSize.x + mapOffset.x)
			{
				if (isGameover == false)
				{
					directing.BreakWallStart();
				}
			}

			score = (int)(paintBlockCount / blockCount * 100.0f);

			colorDrawHeight = score * colorDrawOne;

			if (player.pos.x > goalMapWidth * blockSize.x + mapOffset.x)
			{
				if (isGameover == true)
				{
					player.pos.x = mapOffset.x;
					player.cameraPosX = player.pos.x;
				}
				else
				{
					if (laps >= 2)
					{
						isClear = true;
						stageScore.MaxScore(stageNo, score, medal);
						isStageClear[stageNo] = true;
						directing.scoreEasingStart(XMFLOAT3(0, -400, 0), XMFLOAT3(0, 0, 0), 20);
					}
					else
					{
						//戻り処理開始
						directing.FlyStart(player.pos, XMFLOAT3(player.pos.x / 2, 200.0f, player.pos.z),
							XMFLOAT3(player.pos.x / 4, 200.0f, player.pos.z), XMFLOAT3(0, 0.0f, 0.0f), 200);
						laps++;
					}
				}
			}
			if (player.pos.y <= -70.0f || player.cameraPosX - player.pos.x > 150)
			{
				if (isClear == false)
				{
					isGameover = true;
					directing.ShakeStart(10.0f, 10);
					directing.scoreEasingStart(XMFLOAT3(0, -400, 0), XMFLOAT3(0, 0, 0), 20);
				}
			}

			//ラップ２の戻り処理
			if (directing.pFlyFlag == false)
			{
				player.pos = directing.PFly();
				player.cameraPosX = player.pos.x;
				draw.SetCamera(
					XMFLOAT3(player.pos.x + 130.0f + directing.shakeX, player.pos.y + 30.0f + directing.shakeY, player.pos.z - 170.0f),
					XMFLOAT3(player.pos.x + 100.0f, player.pos.y + 30.0f + directing.shakeY, player.pos.z),
					upVec, MAIN_CAMERA);
				player.rotaMat = XMMatrixRotationZ(XMConvertToRadians(directing.playerAngle));
				if (directing.pFlyTimeRate >= 1.0f)
				{
					for (size_t y = 0; y < MAP_HEIGHT; y++)
					{
						for (size_t x = 0; x < MAP_WIDTH; x++)
						{
							if (map[y][x] == ObjectStatus::BREAK_RING)
							{
								// リングを元に戻す
								map[y][x] = ObjectStatus::RING;
							}
						}
					}

					player.rotaMat = XMMatrixIdentity();
					directing.pFlyFlag = true;
				}
			}

			break;
		default:
			gameStatus = GameStatus::TitleInit;
			break;
		}

		if (Input::IsKey(DIK_LEFT))
		{
			//	isClear = true;
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

		warpCount = 0;

		switch (oldGameStatus)
		{
		case GameStatus::TitleInit:
		case GameStatus::Title:
			draw.SetDrawBlendMode(BLENDMODE_ALPHA);
			DirectDrawing::isDepthWriteBan = false;

			draw.DrawTextrue(0, 0, window_width, window_height, 0.0f, titleGraph, XMFLOAT2(0.0f, 0.0f));
			//draw.DrawString(window_width / 2.0f - 120.0f, window_height / 2.0f + 100.0f, 3.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Please SPACE");
			break;
		case GameStatus::SelectInit:
		case GameStatus::Select:
		{
			draw.SetDrawBlendMode(BLENDMODE_ALPHA);
			DirectDrawing::isDepthWriteBan = false;

			//タイトル戻るボタン
			if (stageSelectTitle == true)
			{
				directing.SelectTitle();
			}
			//ステージセレクト描画
			directing.StageSelectDraw(stageScore.score, stageScore.medal, maxStageCount, window_width, window_height);
		}
		break;
		case GameStatus::MainInit:
		case GameStatus::Main:
			//視覚的スコアの量
			if (player.groundFlag == false)
			{
				draw.DrawTextrue(30.0f, 129.0f, 110.0f, colorDrawHeight, 0, 0, XMFLOAT2(0.0f, 1.0f), XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f));
			}
			else
			{
				draw.DrawTextrue(30.0f, 129.0f, 115.0f, colorDrawHeight, 0, 0, XMFLOAT2(0.0f, 1.0f), player.color);
			}

			//視覚的スコアの外枠
			draw.DrawTextrue(85.0f, 90.0f, 220.0f, 220.0f, 0, playerScoreGraph);

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
					case ObjectStatus::NOPAINT_BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							noPaintGraph
						);
						break;
					case ObjectStatus::FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							boxGraph
						);
						break;
					case ObjectStatus::NOPAINT_FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
							noPaintGraph
						);
						break;
						/*case ObjectStatus::ITEM:
							draw.DrawOBJ(
								itemModel,
								mapPosition,
								XMMatrixIdentity(),
								XMFLOAT3(5.0f, 5.0f, 5.0f),
								XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)
							);
							break;*/
					case ObjectStatus::RING:
						draw.DrawOBJ(
							ringModel,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(10.0f, 10.0f, 10.0f),
							changeColor[ringColor[ringCount % (sizeof(ringColor) / sizeof(ringColor[0]))]]
						);
						ringCount++;
						break;
					case ObjectStatus::BREAK_RING:
						ringCount++;
						break;
					case ObjectStatus::RED_BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::RED],
							boxGraph
						);
						break;
					case ObjectStatus::BLUE_BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::BLUE],
							boxGraph
						);
						break;
					case ObjectStatus::GREEN_BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::GREEN],
							boxGraph
						);
						break;
					case ObjectStatus::YELLOW_BLOCK:
						draw.Draw(
							box,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::YELLOW],
							boxGraph
						);
						break;
					case ObjectStatus::RED_FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::RED],
							boxGraph
						);
						break;
					case ObjectStatus::BLUE_FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::BLUE],
							boxGraph
						);
						break;
					case ObjectStatus::GREEN_FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::GREEN],
							boxGraph
						);
						break;
					case ObjectStatus::YELLOW_FLOOR:
						draw.Draw(
							boxFloor,
							mapPosition,
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[BlockChange::ColorNo::YELLOW],
							boxGraph
						);
						break;
					case ObjectStatus::WARP:
						warp.Draw(warpCount, mapPosition);
						warpCount++;
						break;
					case ObjectStatus::COLLECTION:
						draw.DrawOBJ(
							coinModel,
							mapPosition,
							XMMatrixIdentity(),
							scale_xyz(10.0f)
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
				noPaintGraph
			);
			/*ゴール地点*/
			if (directing.wallFlag == false)
			{
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
			}
			draw.Draw(
				goalBox,
				XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x * 2.0f, -50.0f, 0),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				noPaintGraph
			);

			player.Draw();

			//壊れる壁
			if (directing.wallFlag == true)
			{
				for (int i = 0; i < 20; i++)
				{
					draw.Draw(
						wallBreak,
						XMFLOAT3(goalMapWidth * blockSize.x + mapOffset.x, -30.0f + i * 20, 0),
						XMMatrixIdentity(),
						XMFLOAT3(1.0f, 1.0f, 1.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						breakwallGraph
					);
				}

			}

			directing.wallDraw();
			DirectDrawing::isDepthWriteBan = true;
			draw.SetDrawBlendMode(BLENDMODE_ADD);
			directing.RunDraw();
			directing.RingDraw();
			directing.explosionDraw();
			directing.DoubleJumpDraw();
			draw.SetDrawBlendMode(BLENDMODE_ALPHA);
			DirectDrawing::isDepthWriteBan = false;

			if (isClear == true)
			{
				XMFLOAT3 scorePos = directing.scoreEasing();

				draw.DrawTextrue(scorePos.x, scorePos.y, window_width, window_height, 0, clearGraph, XMFLOAT2(0.0f, 0.0f));
				directing.scoreDraw(score, medal, selectRetryFlag);
				//draw.DrawString(scorePos.x, scorePos.y, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Clear");
				//draw.DrawString(0, 192, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "score:%d", score);
			}
			if (isGameover == true)
			{
				XMFLOAT3 scorePos = directing.scoreEasing();

				draw.DrawTextrue(scorePos.x, scorePos.y, window_width, window_height, 0, gameoverGraph, XMFLOAT2(0.0f, 0.0f));
				//draw.DrawString(window_width / 2 - 120, window_height / 2 - 160, 5.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "GameOver");
				directing.GameOverButtonDraw(scorePos, selectRetryFlag);
			}
#if _DEBUG
			//draw.DrawString(0, 0, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "laps:%d", laps);
			//draw.DrawString(0, 64, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "stage:%d", stageNo + 1);
			draw.DrawString(0, 192, 4.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "score:%d", score);
#endif // _DEBUG
			break;
		default:
			break;
			}
		directing.SceneChangeUpdate();
		directing.SceneChangeDraw();
		// ループの終了処理
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (isLoopEnd == true) { break; }
		if (Input::IsKey(DIK_ESCAPE)) { break; }
		}

	w.WindowEnd();

	return 0;
	}