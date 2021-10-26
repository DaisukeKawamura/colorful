#pragma once
#include <stdlib.h>
#include "./Header/DrawPolygon.h"
using namespace DirectX;
using namespace std;
class Particle
{
public:
	Particle(XMFLOAT3 pos, XMFLOAT3 speed, XMFLOAT3 scale, XMFLOAT4 color,float time);
	~Particle();
	XMFLOAT3 pos;     //���W
	XMFLOAT3 speed;	  //�X�s�[�h
	XMFLOAT3 scale;   //�X�P�[��
	XMFLOAT4 color;   //�F
	XMMATRIX rotaMat; //��]�s��
	float time;
	bool DelFlag;
};