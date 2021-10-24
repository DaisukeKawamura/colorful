#pragma
#include "./Header/Vector3.h"
#include <stdlib.h>
#include <./DirectXTex/d3dx12.h>
#include "./Header/DrawPolygon.h"
#include "./Header/Partcle.h"
#include <vector>
using namespace std;

class Directing
{
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
public:
	DrawPolygon *draw;  //描画用

	//全体初期化
	void Init();
#pragma once パーティクル画像読み込み
public:
	int particlePolygon;//テクスチャ用四角形
	int graph1;			//パーティクルテクスチャその1
	int graph2;			//パーティクルテクスチャその2
	int graph3;			//パーティクルテクスチャその3
	int lap1Graph;      //Lap1の画像
	void ParticleInit(DrawPolygon *draw);
#pragma once 死亡時シェイク
public:
	float shakeX, shakeY;	//シェイクした分の大きさ
private:
	float shakeTime;		//シェイク時間
	int  shakeString;		//シェイクの強さ
	float shkeStringTime;	//減衰
	bool shakeFlag;			//シェイクフラグ
public:
	//シェイク更新
	void ShakeUpdate();
	//シェイクスタート/シェイク時間、シェイクの強さ
	void ShakeStart(float shakeTime, int shakeString);
#pragma once スコアイージング
public:
	Vector3 scoreStart;				//スタート地点
	Vector3 scoreEnd;				//エンド地点
	float scoreEasingTime;
	float scoreMaxTime;				//全体時間[s]
	float scoreTimeRate;				//何％　時間が進んだか(率)
	bool scoreFlag;
	//スコアイージング/スタート位置、終了位置、進む時間
	void scoreEasingStart(XMFLOAT3 start, XMFLOAT3 end, float Time);
	//スコアイージング更新
	XMFLOAT3 scoreEasing();
#pragma once アイテムイージング
	Vector3 itemStart;				//スタート地点
	Vector3 itemEnd;				//エンド地点
	float itemEasingTime;
	float itemMaxTime;				//全体時間[s]
	float itemTimeRate;				//何％　時間が進んだか(率)
	float itemCameraPos;
	bool itemFlag;
	XMFLOAT3 itemScale;
public:
	//アイテムイージング/スタート位置、終了位置、進む時間
	void ItemStart(XMFLOAT3 start, XMFLOAT3 end, float time, float cameraPos);

	XMFLOAT3 ItemUpdate(XMFLOAT3 cameraPos);
#pragma once プレイヤーRunパーティクル
	vector<Particle *> run;

	void RunUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RunDraw();
#pragma once 色変わる瞬間のパーティクル
	vector<Particle *>ring;
	bool ringFlag;

	int ringTime = 0;

	void RingStart();

	void RingUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RingDraw();
#pragma 1週目演出
	Vector3 lap1Start;				//スタート地点
	Vector3 lap1End;				//エンド地点
	float lap1EasingTime;
	float lap1MaxTime;				//全体時間[s]
	float lap1TimeRate;				//何％　時間が進んだか(率)
	float lap1CameraPos;
	bool lap1Flag;

	void Lap1();

	void Lap1Start();

	void Lap1Draw();

#pragma 2週目演出




};
//XMFLOAT3同士の計算
const  DirectX::XMFLOAT3 operator+(const  DirectX::XMFLOAT3 &lhs, const  DirectX::XMFLOAT3 rhs);
const  DirectX::XMFLOAT3 operator-(const  DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 rhs);
//XMFLOAT4同士の計算
const  DirectX::XMFLOAT4 operator+(const  DirectX::XMFLOAT4 &lhs, const  DirectX::XMFLOAT4 rhs);
const  DirectX::XMFLOAT4 operator-(const  DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs);