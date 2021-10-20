#include "./Header/Player.h"

#define scale_xyz(_scale) DirectX::XMFLOAT3(_scale, _scale, _scale)

const float Player::gravity = 0.70f;
const DirectX::XMFLOAT3 Player::gravityAxis = XMFLOAT3(0, -1, 0);

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
	playerTex{},
	jumpPower{},
	jumpPowerDecay{},
	jumpFlag(false),
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

		this->playerObject = this->draw->CreateOBJModel("./Resources/playerobj/playerobj.obj", "./Resources/playerobj/");
		this->playerTex = 0;
	}

	this->pos = XMFLOAT3(0, 0, 0);
	this->oldPos = XMFLOAT3(0, 0, 0);
	this->rotaMat = XMMatrixIdentity();
	this->scale = scale_xyz(2.0f);
	this->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	this->speed = 2.0f;
	this->speedVec = XMFLOAT3(1, 0, 0);
	this->accel = 0.0f;
	this->accelVec = XMFLOAT3(1, 0, 0);
	this->jumpPower = 0.0f;
	this->jumpPowerDecay = 0.0f;
	this->jumpFlag = false;
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
	}

	if (jumpPower == 0 && jumpPowerDecay == 0)
	{//ふわふわ落ちないように
		jumpPowerDecay = 0.5f;
	}

	jumpPower -= jumpPowerDecay;
	

	if (jumpFlag == true && groundFlag == true)
	{
		//赤ブロック踏んだ時
		if (groundColor == red)
		{
			JumpStart(9.0f, 0.5);
		}
		//ノーマルジャンプ
		else
		{
			JumpStart(6.8f, 0.5);
		}
	}
	groundFlag = false;
	groundColor = false;

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
			pos.x += 0.1f;
		}
	}

	collision.Initilize(pos, rotaMat, 5, 5, 5);
}

void Player::Draw()
{
	draw->DrawOBJ(playerObject, pos, rotaMat, scale, color);
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
	case ObjectStatus::RedBLOCK:
	case ObjectStatus::RedFloor:
		groundColor = red;
		break;
	case ObjectStatus::BlueBLOCK:
	case ObjectStatus::BlueFloor:
		groundColor = blue;
		break;
	case ObjectStatus::GreenBLOCK:
	case ObjectStatus::GreenFloor:
		groundColor = green;
		break;
	case ObjectStatus::YellowBLOCK:
	case ObjectStatus::YellowFloor:
		groundColor = yellow;
		break;
	};
}