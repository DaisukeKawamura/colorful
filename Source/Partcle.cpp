#include "./Header/Partcle.h"


Particle::Particle(XMFLOAT3 pos, XMFLOAT3 speed, XMFLOAT3 scale, XMFLOAT4 color, float time)
{
	this->pos = pos;
	this->rotaMat = XMMatrixIdentity();
	this->scale = scale;
	this->color = color;
	this->speed = speed;
	this->time = time;
}

Particle::~Particle()
{
}
