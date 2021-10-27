#pragma once
#include <stdlib.h>
#include "./Header/DrawPolygon.h"
#include "./Header/Vector3.h"
using namespace DirectX;
using namespace std;
class Particle
{
public:
	Particle();
	Particle(Vector3 pos, Vector3 speed, Vector3 scale, XMFLOAT4 color,float time);
	~Particle();
	Vector3 pos;     //���W
	Vector3 speed;   //�X�s�[�h
	Vector3 scale;   //�X�P�[��
	XMFLOAT4 color;   //�F
	XMMATRIX rotaMat; //��]�s��
	float time;
	bool DelFlag;
};