#pragma once
#include <./DirectXTex/d3dx12.h>
#include<DirectXMath.h>
#include"Vector3.h"
#include"LoadCSV.h"
using namespace DirectX;

class OBB
{
protected:
	Vector3 m_Pos;              // 位置
	Vector3 m_NormaDirect[3];   // 方向ベクトル
	FLOAT m_fLength[3];         // 各軸方向の長さ
public:
	//XMFLOAT3 Pos 位置, XMMATRIX rotM 回転行列、FLOAT xの半分の長さ、 FLOAT yの半分の長さ、 FLOAT zの半分の長さ、
	void Initilize(XMFLOAT3 Pos, XMMATRIX rotM, FLOAT xLength, FLOAT yLength, FLOAT zLength);

	Vector3 GetDirect(int elem);   // 指定軸番号の方向ベクトルを取得
	FLOAT GetLen_W(int elem);          // 指定軸方向の長さを取得
	Vector3 GetPos_W();             // 位置を取得
};



class OBBCollision :OBB
{
public:
	static bool ColOBBs(OBB &obb1, OBB &obb2);

	static FLOAT LenSegOnSeparateAxis(Vector3 &Sep, Vector3 &e1, Vector3 &e2, Vector3 e3 = { 0,0,0 });

	//押し戻し処理/ プレイヤーの位置,前のプレイヤーの位置,OBBプレイヤー、OBBブロック,地面フラグ、上のブロック、下のブロック
	static void PushbackPolygon(XMFLOAT3 &position, const XMFLOAT3 &oldPosition, OBB &player, OBB &block, bool &HitDown, const int up = 0, const int down = 0);
	//床下からすり抜け用/ プレイヤーの位置,前のプレイヤーの位置,OBBプレイヤー、OBBブロック
	static void PushbackFloor(XMFLOAT3 &position, const  XMFLOAT3 oldPosition, OBB &player, OBB &block, bool &HitDown);
};

