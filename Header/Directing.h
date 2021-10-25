#pragma
#include "./Header/Vector3.h"
#include <stdlib.h>
#include <./DirectXTex/d3dx12.h>
#include "./Header/DrawPolygon.h"
#include "./Header/Partcle.h"
#include <vector>
using namespace std;

class Directing
{
	// DirectX::���ȗ�
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
public:
	DrawPolygon *draw;  //�`��p

	//�S�̏�����
	void Init();

public:// �p�[�e�B�N���摜�ǂݍ���
	int particlePolygon;//�e�N�X�`���p�l�p�`
	int wallBreak3D;     //����ǃ|���S��
	int graph1;			//�p�[�e�B�N���e�N�X�`������1
	int graph2;			//�p�[�e�B�N���e�N�X�`������2
	int graph3;			//�p�[�e�B�N���e�N�X�`������3
	int lap1Graph;      //Lap1�̉摜
	int lap2Graph;      //Lap2�̉摜
	int wallBreakGraph; //�����
	void ParticleInit(DrawPolygon *draw);

public:// ���S���V�F�C�N
	float shakeX, shakeY;	//�V�F�C�N�������̑傫��
	float shakeTime;		//�V�F�C�N����
	int  shakeString;		//�V�F�C�N�̋���
	float shkeStringTime;	//����
	bool shakeFlag;			//�V�F�C�N�t���O
	//�V�F�C�N�X�V
	void ShakeUpdate();
	//�V�F�C�N�X�^�[�g/�V�F�C�N���ԁA�V�F�C�N�̋���
	void ShakeStart(float shakeTime, int shakeString);

public:// �X�R�A�C�[�W���O
	Vector3 scoreStart;				//�X�^�[�g�n�_
	Vector3 scoreEnd;				//�G���h�n�_
	float scoreEasingTime;
	float scoreMaxTime;				//�S�̎���[s]
	float scoreTimeRate;			//�����@���Ԃ��i�񂾂�(��)
	bool scoreFlag;
	//�X�R�A�C�[�W���O/�X�^�[�g�ʒu�A�I���ʒu�A�i�ގ���
	void scoreEasingStart(XMFLOAT3 start, XMFLOAT3 end, float Time);
	//�X�R�A�C�[�W���O�X�V
	XMFLOAT3 scoreEasing();
public:	//�A�C�e���C�[�W���O
	Vector3 itemStart;				//�X�^�[�g�n�_
	Vector3 itemEnd;				//�G���h�n�_
	float itemEasingTime;
	float itemMaxTime;				//�S�̎���[s]
	float itemTimeRate;				//�����@���Ԃ��i�񂾂�(��)
	float itemCameraPos;
	bool itemFlag;
	XMFLOAT3 itemScale;
	//�A�C�e���C�[�W���O/�X�^�[�g�ʒu�A�I���ʒu�A�i�ގ���
	void ItemStart(XMFLOAT3 start, XMFLOAT3 end, float time, float cameraPos);

	XMFLOAT3 ItemUpdate(XMFLOAT3 cameraPos);
public:	//�v���C���[Run�p�[�e�B�N��
	vector<Particle *> run;

	void RunUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RunDraw();

public:// �F�ς��u�Ԃ̃p�[�e�B�N��
	vector<Particle *>ring;
	bool ringFlag;

	int ringTime = 0;

	void RingStart();

	void RingUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RingDraw();

public://1�T�ډ��o

	Vector3 lap1Start;				//�X�^�[�g�n�_
	Vector3 lap1End;				//�G���h�n�_
	float lap1EasingTime;
	float lap1MaxTime;				//�S�̎���[s]
	float lap1TimeRate;				//�����@���Ԃ��i�񂾂�(��)
	Vector3 lap1Pos;
	bool lap1Flag;

	void Lap1Update(XMFLOAT3 start, XMFLOAT3 end, float time);

	void Lap1Draw();

public:	//2�T�ډ��o
	Vector3 lap2Start;				//�X�^�[�g�n�_
	Vector3 lap2End;				//�G���h�n�_
	float lap2EasingTime;
	float lap2MaxTime;				//�S�̎���[s]
	float lap2TimeRate;				//�����@���Ԃ��i�񂾂�(��)
	Vector3 lap2Pos;
	bool lap2Flag;

	void Lap2Start(XMFLOAT3 start, XMFLOAT3 end, float time);

	void Lap2Update();

	void Lap2Draw();
public://�v���C���[��΂�
	Vector3 playerStart;				//�X�^�[�g�n�_
	Vector3 controlPoint1;				//����_1
	Vector3 controlPoint2;				//����_2
	Vector3 playerEnd;					//�G���h�n�_
	float pFlyEasingTime;
	float pFlyMaxTime;				//�S�̎���[s]
	float pFlyTimeRate;				//�����@���Ԃ��i�񂾂�(��)
	Vector3 pFlyPos;
	bool pFlyFlag;
	void FlyStart(XMFLOAT3 start, XMFLOAT3 controlPoint1, XMFLOAT3 controlPoint2, XMFLOAT3 end, float time);
	XMFLOAT3 PFly();
public:	//����ǂƔ���
	vector<Particle *> wall;
	vector<Particle *>explosion;
	XMFLOAT3 explosionPos;			//�����̏����ʒu
	int wallTime;					//�p�[�e�B�N���o������
	bool wallFlag;
	float playerAngle;				//��΂����̃v���C���[��]

	void BreakWallStart();

	void BreakWall(XMFLOAT3 wallPos, XMFLOAT3 player);

	void wallDraw();

	void explosionDraw();
};
//XMFLOAT3���m�̌v�Z
const  DirectX::XMFLOAT3 operator+(const  DirectX::XMFLOAT3 &lhs, const  DirectX::XMFLOAT3 rhs);
const  DirectX::XMFLOAT3 operator-(const  DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 rhs);
//XMFLOAT4���m�̌v�Z
const  DirectX::XMFLOAT4 operator+(const  DirectX::XMFLOAT4 &lhs, const  DirectX::XMFLOAT4 rhs);
const  DirectX::XMFLOAT4 operator-(const  DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs);