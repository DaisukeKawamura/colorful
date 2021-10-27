#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/LoadCSV.h"
#include "./Header/Vector3.h"
#include "./Header/Partcle.h"

class Warp
{
private: //エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: //定数
	static const int maxWarp = 10; //ワープの最大数

	static const int maxParticleNum = 100; //パーティクルの最大数
	static const float emitRadius;         //出現範囲の半径
	static const float maxTime;            //演出の時間

public: //メンバ関数
	Warp();
	~Warp() {}

	// 初期化
	void Init(DrawPolygon* draw);
	// ワープの初期化
	void WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo);
	// ワープ（warpNo:入口側の番号）
	const XMFLOAT2& WarpPos(const int& warpNo);
	// 演出の更新（warpNo:入口側の番号）
	void ParticleUpdate(const int& warpNo);
	// 描画（warpNo:入口側の番号）
	void Draw(const int& warpNo, const XMFLOAT3& offset);

private: //メンバ変数
	DrawPolygon* draw; //描画用
	int warpModel;

	XMFLOAT2 warpPos[maxWarp]; //ワープの出入口があるマップチップ座標
	bool warpFlag[maxWarp];    //ワープに入れるかどうか
	int warpExit[maxWarp];     //ワープの出口の座標インデックス
	static size_t warpCount;      //ステージ内のワープの総数

	static Particle particle[maxParticleNum * maxWarp]; //パーティクルの一粒の情報
	static int particlePolygon;                         //パーティクルのポリゴンデータ
	static XMFLOAT3 emitPos[maxParticleNum * maxWarp];  //出現場所の保存
	static float time;                                  //経過時間
	static float timeRate[maxParticleNum * maxWarp];    //演出の進捗
};
