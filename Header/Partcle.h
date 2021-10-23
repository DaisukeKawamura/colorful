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
	XMFLOAT3 pos;     //座標
	XMFLOAT3 speed;	  //スピード
	XMFLOAT3 scale;   //スケール
	XMFLOAT4 color;   //色
	XMMATRIX rotaMat; //回転行列
	float time;
	bool DelFlag;
};