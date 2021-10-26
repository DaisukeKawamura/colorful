#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/LoadCSV.h"
#include <DirectXMath.h>
#include <vector>

class Warp
{
private: //�G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: //�����o�֐�
	Warp();
	~Warp() {}

	// ������
	void Init(DrawPolygon* draw);
	// ���[�v�̏�����
	void WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo);
	// ���[�v
	const XMFLOAT3& WarpPos(const int& warpNo);
	// �`��
	void Draw(const XMFLOAT3& offset);

private: //�����o�ϐ�
	DrawPolygon* draw; //�`��p
	int warpModel;

	std::vector<XMFLOAT3> warpPos; //���[�v�̏o����������}�b�v�`�b�v���W
	bool warpFlag[10]; //���[�v�ɓ���邩�ǂ���
	int warpExit[10];  //���[�v�̏o���̍��W�C���f�b�N�X
};
