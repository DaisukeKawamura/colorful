#pragma once
#include <./DirectXTex/d3dx12.h>
#include<DirectXMath.h>
#include"Vector3.h"
#include"LoadCSV.h"
using namespace DirectX;

class OBB
{
protected:
	Vector3 m_Pos;              // �ʒu
	Vector3 m_NormaDirect[3];   // �����x�N�g��
	FLOAT m_fLength[3];         // �e�������̒���
public:
	//XMFLOAT3 Pos �ʒu, XMMATRIX rotM ��]�s��AFLOAT x�̔����̒����A FLOAT y�̔����̒����A FLOAT z�̔����̒����A
	void Initilize(XMFLOAT3 Pos, XMMATRIX rotM, FLOAT xLength, FLOAT yLength, FLOAT zLength);

	Vector3 GetDirect(int elem);   // �w�莲�ԍ��̕����x�N�g�����擾
	FLOAT GetLen_W(int elem);          // �w�莲�����̒������擾
	Vector3 GetPos_W();             // �ʒu���擾
};



class OBBCollision :OBB
{
public:
	static bool ColOBBs(OBB &obb1, OBB &obb2);

	static FLOAT LenSegOnSeparateAxis(Vector3 &Sep, Vector3 &e1, Vector3 &e2, Vector3 e3 = { 0,0,0 });

	//�����߂�����/ �v���C���[�̈ʒu,�O�̃v���C���[�̈ʒu,OBB�v���C���[�AOBB�u���b�N,�n�ʃt���O�A��̃u���b�N�A���̃u���b�N
	static void PushbackPolygon(XMFLOAT3 &position, const XMFLOAT3 &oldPosition, OBB &player, OBB &block, bool &HitDown, const int up = 0, const int down = 0);
	//�������炷�蔲���p/ �v���C���[�̈ʒu,�O�̃v���C���[�̈ʒu,OBB�v���C���[�AOBB�u���b�N
	static void PushbackFloor(XMFLOAT3 &position, const  XMFLOAT3 oldPosition, OBB &player, OBB &block, bool &HitDown);
};

