#pragma once
#include "./Header/LoadCSV.h"
#include <./DirectXTex/d3dx12.h>
#include<DirectXMath.h>
using namespace DirectX;
class BlockChange
{
public:
	//�v���C���[�̐F�Ƀu���b�N��ς���/XMFLOAT4 �v���C���[�̐F
	static int changeBlockPColor(XMFLOAT4 pColor);

	//�v���C���[�̐F�ɏ���ς���/XMFLOAT4 �v���C���[�̐F
	static int changeFloorPColor(XMFLOAT4 pColor);
};