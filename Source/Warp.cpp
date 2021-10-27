#include "./Header/Warp.h"
#include <stdlib.h>

size_t Warp::warpCount = 0;

const float Warp::emitRadius = 50.0f;
const float Warp::maxTime = 100.0f;

Particle Warp::particle[Warp::maxParticleNum * Warp::maxWarp] = {};
int Warp::particlePolygon = 0;
DirectX::XMFLOAT3 Warp::emitPos[Warp::maxParticleNum * Warp::maxWarp] = {};
float Warp::time = 0.0f;
float Warp::timeRate[Warp::maxParticleNum * Warp::maxWarp] = {};

Warp::Warp() :
	draw(nullptr),
	warpModel(0),
	warpPos{},
	warpFlag{},
	warpExit{}
{
}

void Warp::Init(DrawPolygon* draw)
{
	static bool isInit = false;

	if (isInit == false)
	{
		isInit = true;

		this->draw = draw;
		warpModel = this->draw->CreateOBJModel("./Resources/warp/wa-pu.obj", "./Resources/warp/");
		particlePolygon = this->draw->CreateTriangle(
			XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
			XMFLOAT3(+0.0f, +1.0f, 0.0f), XMFLOAT2(0.5f, 0.0f),
			XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)
		);
	}
}

void Warp::WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo)
{
	warpCount = 0;

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (map[y][x] == ObjectStatus::WARP)
			{
				warpPos[warpCount] = XMFLOAT2((float)(x), (float)(y));
				warpCount++;
			}
		}
	}

	static int warpData[20] = { 0 };
	static char* filePath = nullptr;

	switch (stageNo)
	{
	case 3:
		filePath = (char *)"./Resources/stage/warpData4.csv";
		break;
	case 4:
		filePath = (char *)"./Resources/stage/warpData5.csv";
		break;
	case 5:
		filePath = (char *)"./Resources/stage/warpData6.csv";
		break;
	default:
		filePath = (char*)"./Resources/stage/warpData2.csv";
		break;
	}

	if (LoadCSV1D(warpData, maxWarp * 2, filePath, -2) != 0)
	{
		return;
	}

	for (size_t i = 0; i < warpCount * 2; i++)
	{
		if (i < warpCount)
		{
			if (warpData[i])
			{
				warpFlag[i] = true;
			}
			else
			{
				warpFlag[i] = false;
			}
		}
		else
		{
			warpExit[i - warpCount] = warpData[i];
		}
	}

	for (size_t i = 0; i < maxParticleNum * warpCount; i++)
	{
		particle[i].rotaMat = XMMatrixIdentity();
		particle[i].scale = scale_xyz(1.0f);
		particle[i].DelFlag = true;
		timeRate[i] = 0.0f;
	}

	time = 0;
}

const Warp::XMFLOAT2 Warp::WarpPos(const int& warpNo)
{
	if (warpNo < 0 || warpNo >= warpCount)
	{
		return XMFLOAT2(-1.0f, -1.0f);
	}

	if (warpFlag[warpNo])
	{
		return warpPos[warpExit[warpNo]];
	}
	else
	{
		return XMFLOAT2(-1.0f, -1.0f);
	}
}

void Warp::ParticleUpdate(const int& warpNo)
{
	if (warpNo < 0 || warpNo >= warpCount)
	{
		return;
	}

	using namespace DirectX;

	static int angle1 = 0;
	angle1 = rand() % 360;

	// ベジェ曲線用の制御点
	static Vector3 a = {}, b = {}, c = {};

	time += 1.0f;

	for (size_t i = 0; i < maxParticleNum; i++)
	{
		if (particle[i + (warpNo % warpCount) * maxParticleNum].DelFlag == true)
		{
			static int angle2 = 0;
			angle2 = rand() % 180 - 90;

			emitPos[i + (warpNo % warpCount) * maxParticleNum].y = emitRadius * sinf(2 * XM_PI / angle1 * 360) / 2.0f;
			emitPos[i + (warpNo % warpCount) * maxParticleNum].z = emitRadius * cosf(2 * XM_PI / angle1 * 360) / 2.0f;
			particle[i + (warpNo % warpCount) * maxParticleNum].rotaMat = XMMatrixIdentity();
			particle[i + (warpNo % warpCount) * maxParticleNum].rotaMat *= XMMatrixRotationZ(XMConvertToRadians((float)angle2));
			particle[i + (warpNo % warpCount) * maxParticleNum].rotaMat *= XMMatrixRotationX(XMConvertToRadians((float)angle2));
			particle[i + (warpNo % warpCount) * maxParticleNum].DelFlag = false;
			particle[i + (warpNo % warpCount) * maxParticleNum].time = time; //スタートタイム
			timeRate[i] = 0.0f;

			break;
		}
		else
		{
			timeRate[i + (warpNo % warpCount) * maxParticleNum] = (time - particle[i + (warpNo % warpCount) * maxParticleNum].time) / maxTime;

			if (timeRate[i + (warpNo % warpCount) * maxParticleNum] >= 1.0f)
			{
				particle[i + (warpNo % warpCount) * maxParticleNum].DelFlag = true;
				continue;
			}

			a = lerp(
				{ -emitRadius, emitPos[i + (warpNo % warpCount) * maxParticleNum].y, emitPos[i + (warpNo % warpCount) * maxParticleNum].z },
				{ -emitRadius, 0, 0 },
				timeRate[i + (warpNo % warpCount) * maxParticleNum]
			);
			b = lerp({ -emitRadius / 2.0f, 0, 0 }, { emitRadius / 2.0f, 0, 0 }, timeRate[i + (warpNo % warpCount) * maxParticleNum]);
			c = lerp(
				{ emitRadius, 0, 0 },
				{ emitRadius, emitPos[i + (warpNo % warpCount) * maxParticleNum].y, emitPos[i + (warpNo % warpCount) * maxParticleNum].z },
				timeRate[i + (warpNo % warpCount) * maxParticleNum]
			);

			if (warpFlag[warpNo])
			{
				particle[i + (warpNo % warpCount) * maxParticleNum].pos =
					lerp(a, b, timeRate[i + (warpNo % warpCount) * maxParticleNum]);
			}
			else
			{
				particle[i + (warpNo % warpCount) * maxParticleNum].pos =
					lerp(b, c, timeRate[i + (warpNo % warpCount) * maxParticleNum]);
			}
		}
	}
}

void Warp::Draw(const int& warpNo, const XMFLOAT3& offset)
{
	draw->DrawOBJ(warpModel, XMFLOAT3(offset.x, offset.y + 3.0f, offset.z), DirectX::XMMatrixIdentity(), scale_xyz(2.0f));

	if (warpNo < 0 || warpNo >= warpCount)
	{
		return;
	}

	for (size_t i = 0; i < maxParticleNum; i++)
	{
		if (particle[i + (warpNo % warpCount) * maxParticleNum].DelFlag == true)
		{
			continue;
		}

		draw->Draw(
			particlePolygon,
			particle[i + (warpNo % warpCount) * maxParticleNum].pos + Vector3(offset.x, offset.y + 3.0f, offset.z),
			particle[i + (warpNo % warpCount) * maxParticleNum].rotaMat,
			particle[i + (warpNo % warpCount) * maxParticleNum].scale,
			particle[i + (warpNo % warpCount) * maxParticleNum].color
		);
	}
}
