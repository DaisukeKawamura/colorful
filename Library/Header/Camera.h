#pragma once
#include <DirectXMath.h>

class Camera
{
	/*namespace���ȗ�*/
private:
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

	/*�����o�֐�*/
public:
	Camera() {};
	~Camera() {};

	// �L�����N�^�p���s��̐���
	static DirectX::XMMATRIX CreateCamera(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
	// �L�����N�^�p�������s��̐���
	static DirectX::XMMATRIX CreateCameraFix(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
};
