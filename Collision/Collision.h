#pragma once
#include <DirectXMath.h>

class Collision final
{
	/*namespaceñºè»ó™*/
private:
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:
	Collision() {};
	~Collision() {};
public:
	static bool SphereCollision(XMFLOAT3 pos1, float r1, XMFLOAT3 pos2, float r2);
};
