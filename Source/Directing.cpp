#include "./Header/Directing.h"
#define scale_xyz(_scale) DirectX::XMFLOAT3(_scale, _scale, _scale)
void Directing::Init()
{
	//シェイク初期化
	shakeFlag = true;
	shakeX = 0;
	shakeY = 0;
	//スコアイージング初期化
	scoreFlag = true;
	//アイテムイージング初期化
	itemFlag = false;
	//走るパーティクル削除
	for (int i = (int)run.size() - 1; i >= 0; i--)
	{
		delete run[i];
		run.erase(run.begin() + i);
	}
	//色変えパーティクル削除
	for (int i = (int)ring.size() - 1; i >= 0; i--)
	{
		delete ring[i];
		ring.erase(ring.begin() + i);
	}
}
#pragma once パーティクル画像読み込み
void Directing::ParticleInit(DrawPolygon *draw)
{
	static bool isInit = false;
	if (isInit == false)
	{
		this->draw = draw;
		this->graph1 = draw->LoadTextrue(L"./Resources/particle.jpg");
		this->graph2 = draw->LoadTextrue(L"./Resources/effect2.png");
		this->graph3 = draw->LoadTextrue(L"./Resources/effect3.png");
		this->particlePolygon = this->draw->CreateRect(10, 10);
	}
}
#pragma once 死亡時シェイク
void Directing::ShakeUpdate()
{
	//シェイク初期化
	shakeX = 0, shakeY = 0;
	//シェイクが続く時間
	if (shakeTime > 0)
	{
		shakeX = (rand() % shakeString - shakeString / 2) * shkeStringTime;
		shakeY = (rand() % shakeString - shakeString / 2) * shkeStringTime;
		shakeTime -= 1;
		shkeStringTime -= 0.1f;
	}
}

void Directing::ShakeStart(float shakeTime, int shakeString)
{
	if (shakeFlag == true)
	{
		//シェイクする時間
		this->shakeTime = shakeTime;
		//シェイクの強さ
		this->shakeString = shakeString;
		//減衰
		shkeStringTime = shakeTime / 10;
		shakeFlag = false;
	}
}
#pragma once スコアイージング
void Directing::scoreEasingStart(XMFLOAT3 start, XMFLOAT3 end, float time)
{
	if (scoreFlag == true)
	{
		scoreFlag = false;
		this->scoreStart = Vector3(start.x, start.y, start.z);
		this->scoreEnd = Vector3(end.x, end.y, end.z);
		this->scoreMaxTime = time;
		scoreEasingTime = 0;
	}
}

XMFLOAT3 Directing::scoreEasing()
{
	scoreEasingTime++;

	scoreTimeRate = min(scoreEasingTime / scoreMaxTime, 1.0f);
	Vector3 position;

	position = easeOut(scoreStart, scoreEnd, scoreTimeRate);
	/*position = easeIn(start, end, timeRate);*/
	//position = easeInOut(start, end, timeRate);

	return XMFLOAT3(scoreStart.x, position.y, scoreStart.z);
}
#pragma once アイテムイージング
void Directing::ItemStart(XMFLOAT3 start, XMFLOAT3 end, float time, float cameraPos)
{
	itemFlag = true;
	this->itemStart = Vector3(start.x, start.y, start.z);
	this->itemEnd = Vector3(end.x, end.y, end.z);
	this->itemCameraPos = cameraPos;
	this->itemMaxTime = time;
	itemScale = XMFLOAT3(5.0f, 5.0f, 5.0f);
	itemEasingTime = 0;
}

XMFLOAT3 Directing::ItemUpdate(XMFLOAT3 cameraPos)
{
	//カメラにあわせて毎ターン更新
	float cameraSpeed = cameraPos.x - itemCameraPos;
	itemCameraPos += cameraSpeed;
	itemStart.x += cameraSpeed;
	itemEnd.x += cameraSpeed;

	itemEasingTime++;

	itemTimeRate = min(itemEasingTime / itemMaxTime, 1.0f);

	if (itemScale.x > 0.0f)
	{
		itemScale = itemScale - XMFLOAT3(0.05f, 0.05f, 0.05f);
	}

	if (itemTimeRate >= 1.0f)
	{
		itemFlag = false;
	}

	Vector3 position;
	//position = easeOut(itemStart, itemEnd, itemTimeRate);
	//position = easeIn(itemStart, itemEnd, itemTimeRate);
	position = easeInOut(itemStart, itemEnd, itemTimeRate);

	return XMFLOAT3(position.x, position.y, position.z);
}
#pragma once プレイヤーRunパーティクル
void Directing::RunUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor)
{
	//生み出す
	for (int i = 0; i < 5; i++)
	{
		XMFLOAT3 Pos = pPos;
		Pos.x += rand() % 10 - 5;
		Pos.y += rand() % 4 - 2;
		XMFLOAT3 speed = { -2.0f,0.0f,0.0f };

		XMFLOAT4 color = pColor;

		run.push_back(new Particle(Pos, speed, XMFLOAT3(0.5, 0.5, 0.5), color, 50));
	}

	//更新
	for (size_t i = 0; i < run.size(); i++)
	{
		run[i]->pos = run[i]->pos + run[i]->speed;

		if (run[i]->time >= 20)
		{//徐々に大きくする
			run[i]->scale = run[i]->scale + XMFLOAT3(0.05f, 0.05f, 0.05f);
		}
		//色を薄くしていく
		run[i]->color = run[i]->color - XMFLOAT4(0.04f, 0.04f, 0.04f, 0.04f);

		run[i]->time--;

		if (run[i]->time == 0)
		{
			run[i]->DelFlag = false;
		}
	}
	//削除
	for (int i = (int)run.size() - 1; i >= 0; i--)
	{
		if (run[i]->DelFlag == false)
		{
			delete run[i];
			run.erase(run.begin() + i);
		}
	}
}

void Directing::RunDraw()
{
	//描画
	for (size_t i = 0; i < run.size(); i++)
	{
		draw->Draw(particlePolygon, run[i]->pos, run[i]->rotaMat, run[i]->scale, run[i]->color, graph1);
	}
}

#pragma once 色変わる瞬間のパーティクル
void Directing::RingStart()
{
	ringTime = 10;
}

void Directing::RingUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor)
{
	if (ringTime > 0)
	{
		//生み出す
		for (int i = 0; i < 20; i++)
		{
			XMFLOAT3 Pos = pPos;
			Pos.x += rand() % 18 - 9.0f;
			Pos.y -= 2.0f;
			XMFLOAT3 speed = { 2.8f,3.0f,0.0f };

			XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };

			ring.push_back(new Particle(Pos, speed, XMFLOAT3(0.5, 0.5, 0.5), color, 20));
		}
		ringTime--;
	}

	//更新
	for (size_t i = 0; i < ring.size(); i++)
	{
		ring[i]->pos = ring[i]->pos + ring[i]->speed;

		//色を薄くしていく
		ring[i]->color = ring[i]->color - XMFLOAT4{ 0.08f,0.08f,0.08f,0.08f };

		ring[i]->time--;
		if (ring[i]->time == 0)
		{
			ring[i]->DelFlag = false;
		}
	}
	//削除
	for (int i = (int)ring.size() - 1; i >= 0; i--)
	{
		if (ring[i]->DelFlag == false)
		{
			delete ring[i];
			ring.erase(ring.begin() + i);
		}
	}
}

void Directing::RingDraw()
{
	//描画
	for (size_t i = 0; i < ring.size(); i++)
	{
		draw->Draw(particlePolygon, ring[i]->pos, ring[i]->rotaMat, ring[i]->scale, ring[i]->color, graph2);
	}
}

void Directing::Lap1()
{
	if (shakeFlag == true)
	{
		//シェイクする時間
		this->shakeTime = shakeTime;
		//シェイクの強さ
		this->shakeString = shakeString;
		//減衰
		shkeStringTime = shakeTime / 10;
		shakeFlag = false;
	}
}

void Directing::Lap1Start()
{
	//lap1EasingTime++;

	//lap1TimeRate = min(lap1EasingTime / lap1MaxTime, 1.0f);
	//Vector3 position;

	//position = easeOut(lap1Start, lap1End, lap1TimeRate);
	///*position = easeIn(start, end, timeRate);*/
	////position = easeInOut(start, end, timeRate);

	//return XMFLOAT3(lap1Start.x, position.y, lap1Start.z);
}

void Directing::Lap1Draw()
{
	//draw->DrawTextrue(0, 0, 200, 100, 0, lap1Graph, XMFLOAT2(0.0f, 0.0f));
}


const  DirectX::XMFLOAT3 operator+(const  DirectX::XMFLOAT3 &lhs, const  DirectX::XMFLOAT3 rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}

const DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs)
{
	XMFLOAT4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}
