#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/LoadCSV.h"
#include "./Header/Vector3.h"
#include "./Header/Partcle.h"

class Warp
{
private: //�G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: //�萔
	static const int maxWarp = 10; //���[�v�̍ő吔

	static const int maxParticleNum = 100; //�p�[�e�B�N���̍ő吔
	static const float emitRadius;         //�o���͈͂̔��a
	static const float maxTime;            //���o�̎���

public: //�����o�֐�
	Warp();
	~Warp() {}

	// ������
	void Init(DrawPolygon* draw);
	// ���[�v�̏�����
	void WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo);
	// ���[�v�iwarpNo:�������̔ԍ��j
	const XMFLOAT2& WarpPos(const int& warpNo);
	// ���o�̍X�V�iwarpNo:�������̔ԍ��j
	void ParticleUpdate(const int& warpNo);
	// �`��iwarpNo:�������̔ԍ��j
	void Draw(const int& warpNo, const XMFLOAT3& offset);

private: //�����o�ϐ�
	DrawPolygon* draw; //�`��p
	int warpModel;

	XMFLOAT2 warpPos[maxWarp]; //���[�v�̏o����������}�b�v�`�b�v���W
	bool warpFlag[maxWarp];    //���[�v�ɓ���邩�ǂ���
	int warpExit[maxWarp];     //���[�v�̏o���̍��W�C���f�b�N�X
	static size_t warpCount;      //�X�e�[�W���̃��[�v�̑���

	static Particle particle[maxParticleNum * maxWarp]; //�p�[�e�B�N���̈ꗱ�̏��
	static int particlePolygon;                         //�p�[�e�B�N���̃|���S���f�[�^
	static XMFLOAT3 emitPos[maxParticleNum * maxWarp];  //�o���ꏊ�̕ۑ�
	static float time;                                  //�o�ߎ���
	static float timeRate[maxParticleNum * maxWarp];    //���o�̐i��
};
