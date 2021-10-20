#include "./Header/BlockChange.h"

int BlockChange::changeBlockPColor(XMFLOAT4 pColor)
{
	XMFLOAT4 red = { 0.964f, 0.176f, 0.352f, 1.0f };
	XMFLOAT4 yellow = { 0.9f, 0.9f, 0.0f, 1.0f };
	
	//�v���C���[�̐F�Ɠ����������炻�̐F�ɕς���
	if (pColor.x == red.x && pColor.y == red.y && pColor.z == red.z && pColor.w == red.w)
	{//��
		return  ObjectStatus::RedBLOCK;
	}
	else if (pColor.x == yellow.x && pColor.y == yellow.y && pColor.z == yellow.z && pColor.w == yellow.w)
	{//���F
		return ObjectStatus::YellowBLOCK;
	}
	return 0;
}

int BlockChange::changeFloorPColor(XMFLOAT4 pColor)
{
	XMFLOAT4 red = { 0.964f, 0.176f, 0.352f, 1.0f };
	XMFLOAT4 yellow = { 0.9f, 0.9f, 0.0f, 1.0f };

	//�v���C���[�̐F�Ɠ����������炻�̐F�ɕς���
	if (pColor.x == red.x && pColor.y == red.y && pColor.z == red.z && pColor.w == red.w)
	{//��
		return  ObjectStatus::RedFloor;
	}
	else if (pColor.x == yellow.x && pColor.y == yellow.y && pColor.z == yellow.z && pColor.w == yellow.w)
	{//���F
		return ObjectStatus::YellowFloor;
	}
	return 0;
}


