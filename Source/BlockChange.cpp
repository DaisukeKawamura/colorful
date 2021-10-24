#include "./Header/BlockChange.h"

const XMFLOAT4 BlockChange::changeColor[4] = {
	{ 0.964f, 0.176f, 0.352f, 1.0f }, //��
	{ 0.176f, 0.352f, 0.964f, 1.0f }, //��
	{ 0.352f, 0.964f, 0.176f, 1.0f }, //��
	{ 0.9f, 0.9f, 0.0f, 1.0f }        //���F
};

int BlockChange::changeBlockPColor(XMFLOAT4 pColor)
{
	for (int i = 0; i < sizeof(changeColor) / sizeof(changeColor[0]); i++)
	{
		//�v���C���[�̐F�Ɠ����������炻�̐F�ɕς���
		if (pColor.x == changeColor[i].x && pColor.y == changeColor[i].y && pColor.z == changeColor[i].z && pColor.w == changeColor[i].w)
		{
			return  ObjectStatus::RedBLOCK + i;
		}
	}

	return 0;
}

int BlockChange::changeFloorPColor(XMFLOAT4 pColor)
{
	for (int i = 0; i < sizeof(changeColor) / sizeof(changeColor[0]); i++)
	{
		//�v���C���[�̐F�Ɠ����������炻�̐F�ɕς���
		if (pColor.x == changeColor[i].x && pColor.y == changeColor[i].y && pColor.z == changeColor[i].z && pColor.w == changeColor[i].w)
		{
			return  ObjectStatus::RedFloor + i;
		}
	}

	return 0;
}
