#pragma once
#include <DirectXMath.h>

class Camera
{
	/*namespace名省略*/
private:
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

	/*メンバ関数*/
public:
	Camera() {};
	~Camera() {};

	// キャラクタ姿勢行列の生成
	static DirectX::XMMATRIX CreateCamera(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
	// キャラクタ姿勢束縛行列の生成
	static DirectX::XMMATRIX CreateCameraFix(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector);
};
