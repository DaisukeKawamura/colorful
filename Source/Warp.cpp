#include "./Header/Warp.h"

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
	}
}

void Warp::WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo)
{
	warpPos.clear();

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (map[y][x] == ObjectStatus::WARP)
			{
				warpPos.push_back(XMFLOAT3((float)(x), (float)(y), 0.0f));
			}
		}
	}

	static int warpData[20] = { 0 };
	static char* filePath = nullptr;

	switch (stageNo)
	{
	case 1:
		filePath = (char*)"./Resources/stage/warpData2.csv";
		break;
	default:
		filePath = (char*)"./Resources/stage/warpData2.csv";
		break;
	}

	if (LoadCSV1D(warpData, warpPos.size() * 2, filePath, -2) != 0)
	{
		return;
	}

	for (size_t i = 0; i < warpPos.size() * 2; i++)
	{
		if (i < warpPos.size())
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
			warpExit[i - warpPos.size()] = warpData[i];
		}
	}
}

const Warp::XMFLOAT3& Warp::WarpPos(const int& warpNo)
{
	if (warpNo < 0 || warpNo >= warpPos.size())
	{
		return warpPos[warpNo];
	}

	if (warpFlag[warpNo])
	{
		return warpPos[warpExit[warpNo]];
	}
	else
	{
		return warpPos[warpNo];
	}
}

void Warp::Draw(const XMFLOAT3& offset)
{
	draw->DrawOBJ(warpModel, XMFLOAT3(offset.x, offset.y + 3.0f, offset.z), DirectX::XMMatrixIdentity(), scale_xyz(2.0f));
}
