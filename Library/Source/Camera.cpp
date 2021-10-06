#include "./Header/Camera.h"

DirectX::XMMATRIX Camera::CreateCamera(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector)
{
	using namespace DirectX;

	XMVECTOR x, y, z;
	XMMATRIX mat;

	z = target - pos;
	z = XMVector3Normalize(z);
	x = XMVector3Cross(XMVector3Normalize(upVector), z);
	x = XMVector3Normalize(x);
	y = XMVector3Cross(z, x);
	y = XMVector3Normalize(y);

	mat.r[0] = x;
	mat.r[1] = y;
	mat.r[2] = z;
	mat.r[3] = -pos;

	mat.r[0].m128_f32[3] = 0;
	mat.r[1].m128_f32[3] = 0;
	mat.r[2].m128_f32[3] = 0;
	mat.r[3].m128_f32[3] = 1;

	return mat;
}

DirectX::XMMATRIX Camera::CreateCameraFix(const XMVECTOR& pos, const XMVECTOR& target, const XMVECTOR& upVector)
{
	using namespace DirectX;

	XMVECTOR x, y, z, d;
	XMMATRIX mat;

	y = XMVector3Normalize(upVector);
	d = target - pos;
	d = XMVector3Normalize(d);
	x = XMVector3Cross(y, d);
	x = XMVector3Normalize(x);
	z = XMVector3Cross(x, y);
	z = XMVector3Normalize(z);

	mat.r[0] = x;
	mat.r[1] = y;
	mat.r[2] = z;
	mat.r[3] = -pos;

	mat.r[0].m128_f32[3] = 0;
	mat.r[1].m128_f32[3] = 0;
	mat.r[2].m128_f32[3] = 0;
	mat.r[3].m128_f32[3] = 1;

	return mat;
}
