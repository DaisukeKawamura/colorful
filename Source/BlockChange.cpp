#include "./Header/BlockChange.h"

int BlockChange::changeBlockPColor(XMFLOAT4 pColor)
{
	XMFLOAT4 red = { 0.964f, 0.176f, 0.352f, 1.0f };
	XMFLOAT4 yellow = { 0.9f, 0.9f, 0.0f, 1.0f };
	
	//プレイヤーの色と同じだったらその色に変える
	if (pColor.x == red.x && pColor.y == red.y && pColor.z == red.z && pColor.w == red.w)
	{//赤
		return  ObjectStatus::RedBLOCK;
	}
	else if (pColor.x == yellow.x && pColor.y == yellow.y && pColor.z == yellow.z && pColor.w == yellow.w)
	{//黄色
		return ObjectStatus::YellowBLOCK;
	}
	return 0;
}

int BlockChange::changeFloorPColor(XMFLOAT4 pColor)
{
	XMFLOAT4 red = { 0.964f, 0.176f, 0.352f, 1.0f };
	XMFLOAT4 yellow = { 0.9f, 0.9f, 0.0f, 1.0f };

	//プレイヤーの色と同じだったらその色に変える
	if (pColor.x == red.x && pColor.y == red.y && pColor.z == red.z && pColor.w == red.w)
	{//赤
		return  ObjectStatus::RedFloor;
	}
	else if (pColor.x == yellow.x && pColor.y == yellow.y && pColor.z == yellow.z && pColor.w == yellow.w)
	{//黄色
		return ObjectStatus::YellowFloor;
	}
	return 0;
}


