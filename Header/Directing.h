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

public:// パーティクル画像読み込み
	int particlePolygon;//テクスチャ用四角形
	int wallBreak3D;     //壊れる壁ポリゴン
	int graph1;			//パーティクルテクスチャその1
	int graph2;			//パーティクルテクスチャその2
	int graph3;			//パーティクルテクスチャその3
	int lap1Graph;      //Lap1の画像
	int lap2Graph;      //Lap2の画像
	int wallBreakGraph; //壊れる壁
	void ParticleInit(DrawPolygon *draw);

public:// 死亡時シェイク
	float shakeX, shakeY;	//シェイクした分の大きさ
	float shakeTime;		//シェイク時間
	int  shakeString;		//シェイクの強さ
	float shkeStringTime;	//減衰
	bool shakeFlag;			//シェイクフラグ
	//シェイク更新
	void ShakeUpdate();
	//シェイクスタート/シェイク時間、シェイクの強さ
	void ShakeStart(float shakeTime, int shakeString);

public:// スコアイージング
	Vector3 scoreStart;				//スタート地点
	Vector3 scoreEnd;				//エンド地点
	float scoreEasingTime;
	float scoreMaxTime;				//全体時間[s]
	float scoreTimeRate;			//何％　時間が進んだか(率)
	bool scoreFlag;
	//スコアイージング/スタート位置、終了位置、進む時間
	void scoreEasingStart(XMFLOAT3 start, XMFLOAT3 end, float Time);
	//スコアイージング更新
	XMFLOAT3 scoreEasing();
public:	//アイテムイージング
	Vector3 itemStart;				//スタート地点
	Vector3 itemEnd;				//エンド地点
	float itemEasingTime;
	float itemMaxTime;				//全体時間[s]
	float itemTimeRate;				//何％　時間が進んだか(率)
	float itemCameraPos;
	bool itemFlag;
	XMFLOAT3 itemScale;
	//アイテムイージング/スタート位置、終了位置、進む時間
	void ItemStart(XMFLOAT3 start, XMFLOAT3 end, float time, float cameraPos);

	XMFLOAT3 ItemUpdate(XMFLOAT3 cameraPos);
public:	//プレイヤーRunパーティクル
	vector<Particle *> run;

	void RunUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RunDraw();

public:// 色変わる瞬間のパーティクル
	vector<Particle *>ring;
	bool ringFlag;

	int ringTime = 0;

	void RingStart();

	void RingUpdate(XMFLOAT3 pPos, XMFLOAT4 pColor);

	void RingDraw();

public://1週目演出

	Vector3 lap1Start;				//スタート地点
	Vector3 lap1End;				//エンド地点
	float lap1EasingTime;
	float lap1MaxTime;				//全体時間[s]
	float lap1TimeRate;				//何％　時間が進んだか(率)
	Vector3 lap1Pos;
	bool lap1Flag;

	void Lap1Update(XMFLOAT3 start, XMFLOAT3 end, float time);

	void Lap1Draw();

public:	//2週目演出
	Vector3 lap2Start;				//スタート地点
	Vector3 lap2End;				//エンド地点
	float lap2EasingTime;
	float lap2MaxTime;				//全体時間[s]
	float lap2TimeRate;				//何％　時間が進んだか(率)
	Vector3 lap2Pos;
	bool lap2Flag;

	void Lap2Start(XMFLOAT3 start, XMFLOAT3 end, float time);

	void Lap2Update();

	void Lap2Draw();
public://プレイヤー飛ばし
	Vector3 playerStart;				//スタート地点
	Vector3 controlPoint1;				//制御点1
	Vector3 controlPoint2;				//制御点2
	Vector3 playerEnd;					//エンド地点
	float pFlyEasingTime;
	float pFlyMaxTime;				//全体時間[s]
	float pFlyTimeRate;				//何％　時間が進んだか(率)
	Vector3 pFlyPos;
	bool pFlyFlag;
	void FlyStart(XMFLOAT3 start, XMFLOAT3 controlPoint1, XMFLOAT3 controlPoint2, XMFLOAT3 end, float time);
	XMFLOAT3 PFly();
public:	//壊れる壁と爆発
	vector<Particle *> wall;
	vector<Particle *>explosion;
	XMFLOAT3 explosionPos;			//爆発の初期位置
	int wallTime;					//パーティクル出す時間
	bool wallFlag;
	float playerAngle;				//飛ばし中のプレイヤー回転

	void BreakWallStart();

	void BreakWall(XMFLOAT3 wallPos, XMFLOAT3 player);

	void wallDraw();

	void explosionDraw();
};
//XMFLOAT3同士の計算
const  DirectX::XMFLOAT3 operator+(const  DirectX::XMFLOAT3 &lhs, const  DirectX::XMFLOAT3 rhs);
const  DirectX::XMFLOAT3 operator-(const  DirectX::XMFLOAT3 &lhs, const DirectX::XMFLOAT3 rhs);
//XMFLOAT4同士の計算
const  DirectX::XMFLOAT4 operator+(const  DirectX::XMFLOAT4 &lhs, const  DirectX::XMFLOAT4 rhs);
const  DirectX::XMFLOAT4 operator-(const  DirectX::XMFLOAT4 &lhs, const DirectX::XMFLOAT4 rhs);