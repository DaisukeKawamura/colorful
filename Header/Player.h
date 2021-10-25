#pragma once
#include <DirectXMath.h>
#include "./Header/DrawPolygon.h"
#include "./Header/OBBCollision.h"
#include <Header/LoadCSV.h>
class Player
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �萔
	static const float gravity;        //�d�͉����x
	static const XMFLOAT3 gravityAxis; //�d�͂���������
private:
	static const XMFLOAT4 changeColor[4];

public: // �����o�֐�
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player() {}

	// ������
	void Init(DrawPolygon* draw);
	// �X�V
	void Update();
	// �`��
	void Draw();

	// �v���C���[�̃I�u�W�F�N�g�f�[�^���擾 (�߂�l��-1�Ŏ��s)
	int GetPlayerObject(const int& color) const;
	//�F�̃u���b�N�𓥂񂾎��̏���
	void ChangeGroundColor(const int map);
	/// <summary>
	/// �W�����v�J�n���̃p���[�̐ݒ�
	/// </summary>
	/// <param name="jumpPower"> �W�����v�p���[ </param>
	/// <param name="jumpPowerDecay"> �W�����v�p���[�̌����� </param>
	void JumpStart(float jumpPower, float jumpPowerDecay);

public: // �����o�ϐ�
	DrawPolygon* draw;  //�`��p

	XMFLOAT3 pos;     //���W
	XMFLOAT3 oldPos;  //1�O�̍��W
	XMMATRIX rotaMat; //��]�s��
	XMFLOAT3 scale;   //�X�P�[��
	XMFLOAT4 color;   //�F

	float speed;       //���x
	XMFLOAT3 speedVec; //���x�����������
	float accel;       //�����x
	XMFLOAT3 accelVec; //�����x�����������

	bool jumpFlag; //�W�����v�t���O
	int  jumpCount; //�W�����v�J�E���g
	bool groundFlag; //�n�ʂɐڂ�����
	int  groundColor; //�ǂ̐F�ɐڂ�����
	OBB collision; //�����蔻��

	float cameraPosX;//�J�����̈ʒu
private:
	int playerObject[4]; //�v���C���[�̃I�u�W�F�N�g�f�[�^

	float jumpPower;      //�W�����v�p���[
	float jumpPowerDecay; //�W�����v�p���[�̌�����

	XMFLOAT3 totalSpeed; //���x��S�đ������킹������
	XMFLOAT3 totalAccel; //�����x��S�đ������킹������

};
//�F���
enum GroundColor
{
	red = 1,
	blue,
	green,
	yellow
};