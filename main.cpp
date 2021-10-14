#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/Input.h"
#include "./Header/SafeDelete.h"
#include "./Header/Player.h"
#include "./Header/OBBCollision.h"
#include "./Header/HP.h"
#include "./Header/LoadCSV.h"

/*�E�B���h�E�T�C�Y*/
const int window_width = 1280; //����
const int window_height = 720; //�c��

const WCHAR title[] = L"title"; //�^�C�g��

const float clearColor[] = { 0.1f,0.1f,0.1f,0.0f }; //�w�i�F

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
	hp.Init(50, 1, 60);

	// �摜�̓ǂݍ���
	int background = draw.LoadTextrue(L"./Resources/background.png");
	int tex1 = draw.LoadTextrue(L"./Resources/tex1.png");
	int ringGraph = draw.LoadTextrue(L"./Resources/ring.png");
	int goalGraph = draw.LoadTextrue(L"./Resources/check.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //�J�����̈ʒu
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //�J�����̒����_
	XMFLOAT3 upVec = { 0, 1, 0 };        //��x�N�g��

	// ���C���J�����̏�����
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// �I�u�W�F�N�g�̐���
	int box = draw.Create3Dbox(20.0f, 5.0f, 20.0f);
	int startBox = draw.Create3Dbox(240.0f, 5.0f, 20.0f);
	int goalBox = draw.Create3Dbox(280.0f, 5.0f, 20.0f);
	int ringPolygon = draw.CreateCircle(10.0f, 32);
	int colorBox = draw.Create3Dbox(5.0f, 20.0f, 20.0f);
	int goalPolygon = draw.CreateRect(100.0f, 20.0f);

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	int map[MAP_HEIGHT][MAP_WIDTH] = {};                //CSV�t�@�C���̕ۑ��ꏊ
	const float blockSize = 20.0f;                      //�X�e�[�W���}�b�v�`�b�v�Ƃ��Ĉ������߂̒萔
	const XMFLOAT3 mapOffset = { 130.0f, 50.0f, 0.0f }; //�X�e�[�W�̕`��J�n�ʒu�i����A�I�u�W�F�N�g�j

	int ringCount = 0; //�F�ς������O�̐�
	int colorWallCount = 0; //�F�t���ǂ̐�
	// �F�ς������O���Ŏg���F
	const XMFLOAT4 changeColor[] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};

	size_t goalMapWidth = 60;

	float angle = 0.0f;

	bool isClear = false;
	bool isGameover = false;
	bool isLoopEnd = false;

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
			hp.Init(50, 1, 60);

			{
				int temp = LoadCSV(map, "./Resources/stage/stage1.csv");

				if (temp != 0)
				{
					isLoopEnd = true;
					break;
				}
			}

			// ���C���J�����̏�����
			//draw.SetCamera(XMFLOAT3(15.0f, 0.0f, -110.0f), XMFLOAT3(), upVec, MAIN_CAMERA);

			isClear = false;
			isGameover = false;
			break;
		case GameStatus::Main:
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

			if (player.pos.x > goalMapWidth * blockSize + mapOffset.x * 2.0f)
			{
				player.speed = 0.0f;
			}

			player.Update();

			// ���C���J�����̍X�V
			if (player.pos.x < goalMapWidth * blockSize + mapOffset.x)
			{
				draw.SetCamera(
					XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z - 170.0f),
					XMFLOAT3(player.pos.x + 100.0f, 0.0f, player.pos.z),
					upVec, MAIN_CAMERA);
			}

			angle += 1.0f;

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
						OBB startOBB, blockOBB, goalOBB;

						startOBB.Initilize(XMFLOAT3(0.0f, -50.0f, 0.0f), XMMatrixIdentity(), 120.0f, 2.5f, 10.0f);
						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);
						goalOBB.Initilize(XMFLOAT3(goalMapWidth * blockSize + mapOffset.x * 2.0f, -50.0f, 0.0f), XMMatrixIdentity(), 140.0f, 2.5f, 10.0f);

						bool isHit = OBBCollision::ColOBBs(player.collision, startOBB);
						bool isHitDown = false;
						if (isHit)
						{//�����߂�����
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, startOBB, isHitDown);
						}
						isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{//�����߂�����
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, blockOBB, isHitDown);
						}
						isHit = OBBCollision::ColOBBs(player.collision, goalOBB);
						if (isHit)
						{//�����߂�����
							OBBCollision::PushbackPolygon(player.pos, player.oldPos, player.collision, goalOBB, isHitDown);
						}
						//�n�ʂɂ������̏���
						if (isHitDown)
						{
							hp.AddDamage(0.01f);
						}
					}
						break;
					case ObjectStatus::ITEM:
					{
						OBB itemOBB;
						itemOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);
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
					case ObjectStatus::COLOR_WALL:
					{
						OBB blockOBB;

						blockOBB.Initilize(XMFLOAT3(
							x * blockSize + mapOffset.x, y * (-blockSize) + mapOffset.y, mapOffset.z
						), XMMatrixIdentity(), 10.0f, 2.5f, 10.0f);

						bool isHit = OBBCollision::ColOBBs(player.collision, blockOBB);
						if (isHit)
						{
							map[y][x] = ObjectStatus::BREAK_COLOR_WALL;
							XMFLOAT4 colorBlock = changeColor[colorWallCount % (sizeof(changeColor) / sizeof(changeColor[0]))];
							//�F���������_���[�W
							if (!(player.color.x == colorBlock.x && player.color.y == colorBlock.y && player.color.z == colorBlock.z))
							{
								hp.AddDamage(5);
							}
						}
						colorWallCount++;
					}
					break;
					case ObjectStatus::BREAK_COLOR_WALL:
						colorWallCount++;
						break;
					default:
						break;
					}
				}
			}

			if (player.pos.x > goalMapWidth * blockSize + mapOffset.x)
			{
				isClear = true;
			}
			if (player.pos.y <= -50.0f)
			{
				isGameover = true;
			}
			if (isClear == false)
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

		/* �`�� */

		// �w�i
		draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		switch (gameStatus)
		{
		case GameStatus::Title:
			break;
		case GameStatus::GameInit:
		case GameStatus::Main:
			draw.Draw(
				startBox,
				XMFLOAT3(0, -50.0f, 0),
				XMMatrixIdentity(),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			);
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
							0
						);
						break;
					case ObjectStatus::ITEM:
						//draw->Draw(x * mapSize + offsetX, y * mapSize + offsetY, graphHandleArray[map[y][x]], transFlag);
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
					case ObjectStatus::COLOR_WALL:
						draw.Draw(
							colorBox,
							XMFLOAT3(
								x * blockSize + mapOffset.x,
								y * (-blockSize) + mapOffset.y,
								mapOffset.z
							),
							XMMatrixIdentity(),
							XMFLOAT3(1.0f, 1.0f, 1.0f),
							changeColor[colorWallCount % (sizeof(changeColor) / sizeof(changeColor[0]))],
							0
						);
						colorWallCount++;
						break;
					case ObjectStatus::BREAK_COLOR_WALL:
						colorWallCount++;
						break;
					default:
						break;
					}
				}
			}
			/*�S�[��*/
			draw.Draw(
				goalPolygon,
				XMFLOAT3(goalMapWidth * blockSize + mapOffset.x, 50.0f, 0.0f),
				XMMatrixRotationY(XMConvertToRadians(90)),
				XMFLOAT3(1.0f, 1.0f, 1.0f),
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				goalGraph
			);
			draw.Draw(
				goalPolygon,
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
				XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			);

			player.Draw();

#if _DEBUG
			if (isClear == true)
			{
				draw.DrawString(0, 0, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "Clear");
			}
			if (isGameover == true)
			{
				draw.DrawString(0, 0, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "GameOver");
			}
			draw.DrawString(0, 32, 2.0f, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), "HP%f", hp.GetCurrentHP());
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