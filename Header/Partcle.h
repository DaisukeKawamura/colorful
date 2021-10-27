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
	Vector3 pos;     //座標
	Vector3 speed;   //スピード
	Vector3 scale;   //スケール
	XMFLOAT4 color;   //色
	XMMATRIX rotaMat; //回転行列
	float time;
	bool DelFlag;
};