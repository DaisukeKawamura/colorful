#pragma once
#include "./Header/LoadCSV.h"
#include <./DirectXTex/d3dx12.h>
#include<DirectXMath.h>
using namespace DirectX;
class BlockChange
{
public:
	//プレイヤーの色にブロックを変える/XMFLOAT4 プレイヤーの色
	static int changeBlockPColor(XMFLOAT4 pColor);

	//プレイヤーの色に床を変える/XMFLOAT4 プレイヤーの色
	static int changeFloorPColor(XMFLOAT4 pColor);
};