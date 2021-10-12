#pragma once
#include "DebugText.h"
#include <./DirectXTex/d3dx12.h>

#define MAIN_CAMERA (0)

class DrawPolygon final : public DebugText
{
public: // �����o�֐�
	// �R���X�g���N�^
	DrawPolygon(const DirectXInit* w);
	// �f�X�g���N�^
	~DrawPolygon() {}

	// �O�p�`�̍쐬
	int CreateTriangle(const XMFLOAT3& vertex1, const DirectX::XMFLOAT2& uv1,
		const XMFLOAT3& vertex2, const DirectX::XMFLOAT2& uv2,
		const XMFLOAT3& vertex3, const DirectX::XMFLOAT2& uv3, const bool& isFill = true);
	// ��`�̍쐬
	int CreateRect(const float& width, const float& height, const bool& isFill = true);
	// �����p�`�̍쐬
	int CreateCircle(const float& r, const size_t& divNum, const bool& isFill = true);
	// �����̂̍쐬
	int Create3Dbox(const float& width, const float& height, const float& depth, const bool& isFill = true);
	// �����p���̍쐬
	int CreateCorn(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// �����p���̍쐬
	int CreateCylinder(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// ���̂̕`�揈��
	int CreateSphere(const float& r, const size_t& divNum, const bool& isFill = true);

	// �e�N�X�`���̃^�C�����O
	void NormalizeUV(const int& polygonData, const int& graphHandle);
	// �e�N�X�`���̈ړ�
	void MoveUV(const int& polygonData, const int& graphHandle, DirectX::XMFLOAT2 speed);

	// �I�u�W�F�N�g�̕`�揈��
	int Draw(const int& polygonData, const XMFLOAT3& position, const XMMATRIX& rotation, const XMFLOAT3& scale,
		const XMFLOAT4& color, const int& graphHandle = 0, const bool& isFill = true,
		const bool& isOrthoGraphic = false, const bool& viewMultiFlag = true, const int& parent = -1);

	// �J�����̍쐬
	int CreateCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector);
	// �J�����̐ݒ�
	void SetCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector,
		const int& cameraNo = MAIN_CAMERA);
	// �J�����̐؂�ւ�
	void ChangeCamera(const int& cameraNo);

	// �X�N���[�����W���烏�[���h���W�����߂�
	DirectX::XMFLOAT3 ScreenToWorld(int x, int y, float z);
	// �r���[�s��̋t�s����擾�i�ړ��ʂ�0�Œ�j
	DirectX::XMMATRIX InverseView();

	// �����̏������p�֐�
	void PolygonLoopEnd();
private:
	// �`��֐��̏�����
	int DrawPolygonInit();

	/*���_���W�ƃC���f�b�N�X�f�[�^�v�Z�p*/
	void Circle(const XMFLOAT3& centerPos, const float& r, const size_t& divNum, const bool& flag,
		Vertex* v, unsigned short* index);

private: // �����o�ϐ�
	int polygonCount; //�I�u�W�F�N�g�̐�
	size_t loopCount; //���[�v������

	XMFLOAT3 lightVec; //����
	XMFLOAT3 light;    //����

	vector<XMMATRIX> matView; //�r���[�ϊ��s��(�J����)
	int cameraNo;             //�J�����̔ԍ��i�ŏ���MAIN_CAMERA���w���Ă���j

};
