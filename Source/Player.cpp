#include "./Header/Player.h"
#include "./Header/BlockChange.h"

const float Player::gravity = 0.70f;
const DirectX::XMFLOAT3 Player::gravityAxis = XMFLOAT3(0, -1, 0);

const XMFLOAT4 Player::changeColor[4] = {
	BlockChange::changeColor[0],
	BlockChange::changeColor[1],
	BlockChange::changeColor[2],
	BlockChange::changeColor[3],
};

Player::Player() :
	draw(nullptr),
	pos{},
	oldPos{},
	cameraPosX(0),
	rotaMat{},
	scale{},
	color{},
	speed{},
	speedVec{},
	accel{},
	accelVec{},
	collision{},
	playerObject{},
	jumpPower{},
	jumpPowerDecay{},
	jumpFlag(false),
	jumpCount(1),
	groundFlag(false),
	groundColor(0),
	totalSpeed{},
	totalAccel{}
{
}

void Player::Init(DrawPolygon *draw)
{
	using namespace DirectX;

	static bool isInit = false;

	if (isInit == false)
	{
		this->draw = draw;

		this->playerObject[BlockChange::ColorNo::RED] = this->draw->CreateOBJModel("./Resources/playerobj/jumpmode.obj", "./Resources/playerobj/");
		this->playerObject[BlockChange::ColorNo::BLUE] = this->draw->CreateOBJModel("./Resources/playerobj/speed01.obj", "./Resources/playerobj/");
		this->playerObject[BlockChange::ColorNo::GREEN] = this->draw->CreateOBJModel("./Resources/playerobj/kan1.obj", "./Resources/playerobj/");
		this->playerObject[BlockChange::ColorNo::YELLOW] = this->draw->CreateOBJModel("./Resources/playerobj/skyjump.obj", "./Resources/playerobj/");
	}

	this->pos = XMFLOAT3(0, 0, 0);
	this->oldPos = XMFLOAT3(0, 0, 0);
	this->rotaMat = XMMatrixIdentity();
	this->scale = scale_xyz(4.0f);
	this->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	this->speed = 2.8f;
	this->speedVec = XMFLOAT3(1, 0, 0);
	this->accel = 0.0f;
	this->accelVec = XMFLOAT3(1, 0, 0);
	this->jumpPower = 0.0f;
	this->jumpPowerDecay = 0.0f;
	this->jumpFlag = false;
	this->jumpCount = 1;
	this->totalSpeed = XMFLOAT3(0, 0, 0);
	this->totalAccel = XMFLOAT3(0, 0, 0);

	this->cameraPosX = pos.x;
}

void Player::Update()
{
	oldPos = pos;

	if (jumpPower < -5.0f)
	{
		jumpPowerDecay = 0;
	}
	if (groundFlag == true)
	{
		jumpPower = 0;

		// 黄ブロック踏んだ時
		if (color.x == changeColor[BlockChange::ColorNo::YELLOW].x &&
			color.y == changeColor[BlockChange::ColorNo::YELLOW].y &&
			color.z == changeColor[BlockChange::ColorNo::YELLOW].z &&
			color.w == changeColor[BlockChange::ColorNo::YELLOW].w)
		{
			jumpCount = 2;
		}
		// ノーマルジャンプ
		else
		{
			jumpCount = 1;
		}
	}

	if (jumpPower == 0 && jumpPowerDecay == 0)
	{
		// ふわふわ落ちないように
		jumpPowerDecay = 0.5f;
	}

	jumpPower -= jumpPowerDecay;

	if (jumpFlag == true && jumpCount > 0)
	{
		// 赤ブロック踏んだ時
		if (color.x == changeColor[BlockChange::ColorNo::RED].x &&
			color.y == changeColor[BlockChange::ColorNo::RED].y &&
			color.z == changeColor[BlockChange::ColorNo::RED].z &&
			color.w == changeColor[BlockChange::ColorNo::RED].w)
		{
			JumpStart(9.0f, 0.5);
		}
		// ノーマルジャンプ
		else
		{
			JumpStart(6.8f, 0.5);
		}
		jumpCount--;
	}
	groundFlag = false;
	groundColor = false;

	// 青ブロック踏んだ時
	if (color.x == changeColor[BlockChange::ColorNo::BLUE].x &&
		color.y == changeColor[BlockChange::ColorNo::BLUE].y &&
		color.z == changeColor[BlockChange::ColorNo::BLUE].z &&
		color.w == changeColor[BlockChange::ColorNo::BLUE].w)
	{
		speed = 2.8f * 1.5f;
	}
	//ノーマルスピード
	else
	{
		speed = 2.8f;
	}

	oldPos = pos;

	// accelVecを正規化
	float accelVecLen = sqrtf(
		(accelVec.x * accelVec.x) +
		(accelVec.y * accelVec.y) +
		(accelVec.z * accelVec.z)
	);
	accelVec.x /= accelVecLen;
	accelVec.y /= accelVecLen;
	accelVec.z /= accelVecLen;

	totalAccel.x += (accelVec.x * accel);
	totalAccel.y += (accelVec.y * accel);
	totalAccel.z += (accelVec.z * accel);

	// speedVecを正規化
	float speedVecLen = sqrtf(
		(speedVec.x * speedVec.x) +
		(speedVec.y * speedVec.y) +
		(speedVec.z * speedVec.z)
	);
	speedVec.x /= speedVecLen;
	speedVec.y /= speedVecLen;
	speedVec.z /= speedVecLen;

	totalSpeed = XMFLOAT3(0, 0, 0);
	totalSpeed.x = (speedVec.x * speed) + (-gravityAxis.x * jumpPower);
	totalSpeed.y = (speedVec.y * speed) + (-gravityAxis.y * jumpPower);
	totalSpeed.z = (speedVec.z * speed) + (-gravityAxis.z * jumpPower);
	totalSpeed.x += gravityAxis.x * gravity;
	totalSpeed.y += gravityAxis.y * gravity;
	totalSpeed.z += gravityAxis.z * gravity;

	// posに加算
	pos.x += totalAccel.x + totalSpeed.x;
	pos.y += totalAccel.y + totalSpeed.y;
	pos.z += totalAccel.z + totalSpeed.z;

	//プレイヤーの移動速度に合わせる
	cameraPosX += totalAccel.x + totalSpeed.x;
	//カメラから見た定位置まで戻る/////////
	if (pos.x < cameraPosX)
	{
		if (oldPos.x < pos.x)
		{
			pos.x += 0.3f;
		}
	}

	collision.Initilize(pos, rotaMat, 5, 5, 5);
}

void Player::Draw()
{
	if (color.x == changeColor[BlockChange::ColorNo::RED].x &&
		color.y == changeColor[BlockChange::ColorNo::RED].y &&
		color.z == changeColor[BlockChange::ColorNo::RED].z &&
		color.w == changeColor[BlockChange::ColorNo::RED].w)
	{
		draw->DrawOBJ(playerObject[BlockChange::ColorNo::RED], XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), rotaMat, scale);
	}
	else if (color.x == changeColor[BlockChange::ColorNo::BLUE].x &&
		color.y == changeColor[BlockChange::ColorNo::BLUE].y &&
		color.z == changeColor[BlockChange::ColorNo::BLUE].z &&
		color.w == changeColor[BlockChange::ColorNo::BLUE].w)
	{
		draw->DrawOBJ(playerObject[BlockChange::ColorNo::BLUE], XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), rotaMat, scale);
	}
	else if (color.x == changeColor[BlockChange::ColorNo::GREEN].x &&
		color.y == changeColor[BlockChange::ColorNo::GREEN].y &&
		color.z == changeColor[BlockChange::ColorNo::GREEN].z &&
		color.w == changeColor[BlockChange::ColorNo::GREEN].w)
	{
		draw->DrawOBJ(playerObject[BlockChange::ColorNo::GREEN], XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), rotaMat, scale);
	}
	else
	{
		draw->DrawOBJ(playerObject[BlockChange::ColorNo::YELLOW], XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), rotaMat, scale);
	}
}

void Player::JumpStart(float jumpPower, float jumpPowerDecay)
{
	this->jumpPower = jumpPower;
	this->jumpPower += gravity;
	this->jumpPowerDecay = jumpPowerDecay;
}

void Player::ChangeGroundColor(const int map)
{
	switch (map)
	{
		//色ごとの効果
	case ObjectStatus::RED_BLOCK:
	case ObjectStatus::RED_FLOOR:
		groundColor = red;
		break;
	case ObjectStatus::BLUE_BLOCK:
	case ObjectStatus::BLUE_FLOOR:
		groundColor = blue;
		break;
	case ObjectStatus::GREEN_BLOCK:
	case ObjectStatus::GREEN_FLOOR:
		groundColor = green;
		break;
	case ObjectStatus::YELLOW_BLOCK:
	case ObjectStatus::YELLOW_FLOOR:
		groundColor = yellow;
		break;
	};
}

int Player::GetPlayerObject(const int& color) const
{
	if (color < 0 || color >= sizeof(BlockChange::changeColor) / sizeof(BlockChange::changeColor[0]))
	{
		return -1;
	}

	return playerObject[color];
}