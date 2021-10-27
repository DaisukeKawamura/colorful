#include "./Header/Partcle.h"


Particle::Particle()
{
	this->pos = { 0.0f, 0.0f, 0.0f };
	this->rotaMat = XMMatrixIdentity();
	this->scale = { 1.0f, 1.0f, 1.0f };
	this->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->speed = { 1.0f, 0.0f, 0.0f };
	this->time = 0.0f;
	this->DelFlag = true;
}

Particle::Particle(Vector3 pos, Vector3 speed, Vector3 scale, XMFLOAT4 color, float time)
{
	this->pos = pos;
	this->rotaMat = XMMatrixIdentity();
	this->scale = scale;
	this->color = color;
	this->speed = speed;
	this->time = time;
	this->DelFlag = true;
}

Particle::~Particle()
{
}
