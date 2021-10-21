#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
#include "./Header/LoadCSV.h"
#include "./Header/BlockChange.h"
/*�E�B���h�E�T�C�Y*/
const int window_width = 1280; //����
const int window_height = 720; //�c��

const WCHAR title[] = L"title"; //�^�C�g��

const float clearColor[] = { 0.1f,0.1f,0.3f,0.0f }; //�w�i�F

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	enum GameStatus
	{
		Title,    //�^�C�g��
		GameInit, //Main�̏�����
		Main,     //�Q�[�����C��
		Config    //�ݒ�
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

	// �摜�̓ǂݍ���
	int background = draw.LoadTextrue(L"./Resources/background.png");
	int tex1 = draw.LoadTextrue(L"./Resources/tex1.png");
	int ringGraph = draw.LoadTextrue(L"./Resources/ring.png");
	int goalGraph = draw.LoadTextrue(L"./Resources/check.png");
	int boxGraph = draw.LoadTextrue(L"./Resources/box.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //�J�����̈ʒu
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //�J�����̒����_
	XMFLOAT3 upVec = { 0, 1, 0 };        //��x�N�g��

	// ���C���J�����̏�����
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// �I�u�W�F�N�g�̐���
	int box = draw.Create3Dbox(20.0f, 20.0f, 20.0f);       //�u���b�N
	int boxFloor = draw.Create3Dbox(20.0f, 5.0f, 20.0f);   //��
	int startBox = draw.Create3Dbox(240.0f, 20.0f, 20.0f); //�X�^�[�g�����̏�
	int goalBox = draw.Create3Dbox(280.0f, 20.0f, 20.0f);  //�S�[�������̏�
	int ringPolygon = draw.CreateCircle(10.0f, 16);        //�����O
	int colorBox = draw.Create3Dbox(5.0f, 20.0f, 20.0f);   //�F����v���Ă���Δj��o����u���b�N
	int goalFlag = draw.CreateRect(100.0f, 20.0f);         //�S�[���̊�

	// ���f���̓ǂݍ���
	int itemModel = draw.CreateOBJModel("./Resources/item/4.obj", "./Resources/item/"); //�A�C�e��

	// �e�N�X�`���̃^�C�����O
	draw.NormalizeUV(startBox, boxGraph);
	draw.NormalizeUV(box, boxGraph);
	draw.NormalizeUV(boxFloor, boxGraph);
	draw.NormalizeUV(goalBox, boxGraph);

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int map[MAP_HEIGHT][MAP_WIDTH] = {};                //CSV�t�@�C���̕ۑ��ꏊ
	const float blockSize = 20.0f;                      //�X�e�[�W���}�b�v�`�b�v�Ƃ��Ĉ������߂̒萔
	const XMFLOAT3 mapOffset = { 130.0f, 50.0f, 0.0f }; //�X�e�[�W�̕`��J�n�ʒu�i����A�I�u�W�F�N�g�j

	const float floorOffsetY = 10.0f;					//����Y���̈ʒu

	int mapColor[MAP_HEIGHT][MAP_WIDTH] = {};			//�ς��F�̏���ێ�

	int ringCount = 0; //�F�ς������O�̐�
	int colorWallCount = 0; //�F�t���ǂ̐�
	// �F�ς������O���Ŏg���F
	const XMFLOAT4 changeColor[] = {
		{ 0.964f, 0.176f, 0.352f, 1.0f },
		{ 0.9f, 0.9f, 0.0f, 1.0f }
	};

	size_t goalMapWidth = 90;

	const XMFLOAT3 startPlayerPos = { 130, -25, 0 }; //�v���C���[�̃X�^�[�g�ʒu

	int laps = 1; //����

	bool isClear = false;    //�N���A���ǂ���
	bool isGameover = false; //�Q�[���I�[�o�[���ǂ���

	bool isHit = false;
	bool isLoopEnd = false; //�������[�v�𔲂��邩�ǂ���

	//int score = 0;

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		Input::Update();

		/* �X�V */

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

			// ���C���J�����̏�����
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

			// ���C���J�����̍X�V
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
				{//�����߂�����
					OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, startOBB, isHit);
					player.groundFlag = true;
				}
				isHit = OBBCollision::ColOBBs(player.collision, goalOBB);
				if (isHit)
				{//�����߂�����
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
						{//�����߂�����
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
						}
						//�n�ʂɂ������̏���
						if (isHitDown)
						{
							hp.AddDamage(0.01f);
							player.groundFlag = true;
							//�ς��F�̏���ێ�
							mapColor[y][x] = BlockChange::changeBlockPColor(player.color);
						}
						else
						{
							//�n�ʂ��痣�ꂽ��ێ����Ă����F�ɕς���
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
						{//�����߂�����
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
						}
						//�n�ʂɂ������̏���
						if (isHitDown)
						{
							player.ChangeGroundColor(map[y][x]);
							hp.AddDamage(0.01f);
							player.groundFlag = true;
							//�ς��F�̏���ێ�
							mapColor[y][x] = BlockChange::changeBlockPColor(player.color);
						}
						else
						{
							//�n�ʂ��痣�ꂽ��ێ����Ă����F�ɕς���
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
						{//�����߂�����
							OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
						}
						//�n�ʂɐG�ꂽ��̏���
						if (HitDown)
						{
							player.groundFlag = true;
							//�ς��F��ێ�
							mapColor[y][x] = BlockChange::changeFloorPColor(player.color);

							hp.AddDamage(0.005);
						}
						else
						{
							//�v���C���[�̐F�ɍ��킹�ău���b�N��ς���
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
						{//�����߂�����
							OBBCollision::PushbackFloor(player.pos, player.oldPos, player.collision, blockOBB, HitDown);
						}
						//�n�ʂɐG�ꂽ��̏���
						if (HitDown)
						{
							player.groundFlag = true;
							player.ChangeGroundColor(map[y][x]);
							//�ς��F��ێ�
							mapColor[y][x] = BlockChange::changeFloorPColor(player.color);

							hp.AddDamage(0.005);
						}
						else
						{
							//�v���C���[�̐F�ɍ��킹�ău���b�N��ς���
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

		/* �`�� */

		// �w�i
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
			/*�X�^�[�g�n�_*/
			draw.Draw(
				startBox,
				XMFLOAT3(0.0f, -50.0f, 0.0f),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				boxGraph
			);
			/*�S�[���n�_*/
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

		// ���[�v�̏I������
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (isLoopEnd == true) { break; }
		if (Input::IsKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}