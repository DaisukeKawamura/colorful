#include "Player.h"
#include "./Header/Input.h"

const float Player::gravity = 0.49f;
const DirectX::XMFLOAT3 Player::gravityAxis = XMFLOAT3(0, -1, 0);

Player::Player() :
	draw(nullptr),
	pos{},
	rotaMat{},
	scale{},
	color{},
	speed{},
	speedVec{},
	accel{},
	accelVec{},
	playerObject{},
	playerTex{},
	jumpPower{},
	jumpPowerDecay{},
	totalSpeed{},
	totalAccel{}
{
}

void Player::Init(DrawPolygon* draw)
{
	using namespace DirectX;

	static bool isInit = false;

	if (isInit == false)
	{
		this->draw = draw;

		this->playerObject = this->draw->Create3Dbox(10, 10, 10);
		this->playerTex = 0;
	}

	this->pos = XMFLOAT3(0, 0, 0);
	this->rotaMat = XMMatrixIdentity();
	this->scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	this->speed = 0.0f;
	this->speedVec = XMFLOAT3(1, 0, 0);
	this->accel = 0.0f;
	this->accelVec = XMFLOAT3(1, 0, 0);
	this->jumpPower = 0.0f;
	this->jumpPowerDecay = 0.0f;
	this->totalSpeed = XMFLOAT3(0, 0, 0);
	this->totalAccel = XMFLOAT3(0, 0, 0);
}

void Player::Update()
{
	jumpPower -= jumpPowerDecay;
	if (jumpPower < 0.0f)
	{
		jumpPower = 0.0f;
	}

	if (Input::IsKeyTrigger(DIK_SPACE))
	{
		JumpStart(5.0f, 1.0f);
	}

	// accelVec‚ð³‹K‰»
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

	// speedVec‚ð³‹K‰»
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
	//totalSpeed.x += gravityAxis.x * gravity;
	//totalSpeed.y += gravityAxis.y * gravity;
	//totalSpeed.z += gravityAxis.z * gravity;

	// pos‚É‰ÁŽZ
	pos.x += totalAccel.x + totalSpeed.x;
	pos.y += totalAccel.y + totalSpeed.y;
	pos.z += totalAccel.z + totalSpeed.z;
}

void Player::Draw()
{
	draw->Draw(playerObject, pos, rotaMat, scale, color, playerTex);
}

void Player::JumpStart(float jumpPower, float jumpPowerDecay)
{
	this->jumpPower = jumpPower;
	this->jumpPower += gravity;
	this->jumpPowerDecay = jumpPowerDecay;
}
