#pragma once
#include <DirectXMath.h>
#include "./Header/DrawPolygon.h"
#include "./Header/OBBCollision.h"

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

public: // �����o�֐�
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	~Player() {};

	// ������
	void Init(DrawPolygon* draw);
	// �X�V
	void Update();
	// �`��
	void Draw();

	// �v���C���[�̒��_�f�[�^���擾
	const int& GetPlayerObject() { return playerObject; }
	// �v���C���[�̃e�N�X�`�����擾
	const int& GetPlayerTex() { return playerTex; }

	/// <summary>
	/// �W�����v�J�n���̃p���[�̐ݒ�
	/// </summary>
	/// <param name="jumpPower"> �W�����v�p���[ </param>
	/// <param name="jumpPowerDecay"> �W�����v�p���[�̌����� </param>
	void JumpStart(float jumpPower, float jumpPowerDecay);

public: // �����o�ϐ�
	DrawPolygon* draw;  //�`��p

	XMFLOAT3 pos;     //���W
	XMMATRIX rotaMat; //��]�s��
	XMFLOAT3 scale;   //�X�P�[��
	XMFLOAT4 color;   //�F

	float speed;       //���x
	XMFLOAT3 speedVec; //���x�����������
	float accel;       //�����x
	XMFLOAT3 accelVec; //�����x�����������

	OBB collision; //�����蔻��
private:
	int playerObject; //�v���C���[�̒��_�f�[�^
	int playerTex;    //�v���C���[�̃e�N�X�`��

	float jumpPower;      //�W�����v�p���[
	float jumpPowerDecay; //�W�����v�p���[�̌�����

	XMFLOAT3 totalSpeed; //���x��S�đ������킹������
	XMFLOAT3 totalAccel; //�����x��S�đ������킹������

};
