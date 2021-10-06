#include "Collision.h"

bool Collision::SphereCollision(XMFLOAT3 pos1, float r1, XMFLOAT3 pos2, float r2)
{
	float d = ((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
		((pos1.y - pos2.y) * (pos1.y - pos2.y)) +
		((pos1.z - pos2.z) * (pos1.z - pos2.z));

	return d < ((r1 + r2) * (r1 + r2));
}
