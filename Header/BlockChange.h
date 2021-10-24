#pragma once
#include "./Header/LoadCSV.h"
#include <./DirectXTex/d3dx12.h>
#include <DirectXMath.h>
using namespace DirectX;
class BlockChange
{
public:
	// 色情報
	enum ColorNo
	{
		RED,
		BLUE,
		GREEN,
		YELLOW
	};
public:
	static const XMFLOAT4 changeColor[4]; //色
public:
	//プレイヤーの色にブロックを変える/XMFLOAT4 プレイヤーの色
	static int changeBlockPColor(XMFLOAT4 pColor);

	//プレイヤーの色に床を変える/XMFLOAT4 プレイヤーの色
	static int changeFloorPColor(XMFLOAT4 pColor);
};