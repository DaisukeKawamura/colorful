#pragma once
#include <DirectXMath.h>
#include "./Header/DrawPolygon.h"
#include "./Header/OBBCollision.h"

class Player
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	static const float gravity;        //重力加速度
	static const XMFLOAT3 gravityAxis; //重力が加わる方向

public: // メンバ関数
	// コンストラクタ
	Player();
	// デストラクタ
	~Player() {};

	// 初期化
	void Init(DrawPolygon* draw);
	// 更新
	void Update();
	// 描画
	void Draw();

	// プレイヤーの頂点データを取得
	const int& GetPlayerObject() { return playerObject; }
	// プレイヤーのテクスチャを取得
	const int& GetPlayerTex() { return playerTex; }

	/// <summary>
	/// ジャンプ開始時のパワーの設定
	/// </summary>
	/// <param name="jumpPower"> ジャンプパワー </param>
	/// <param name="jumpPowerDecay"> ジャンプパワーの減衰量 </param>
	void JumpStart(float jumpPower, float jumpPowerDecay);

public: // メンバ変数
	DrawPolygon* draw;  //描画用

	XMFLOAT3 pos;     //座標
	XMMATRIX rotaMat; //回転行列
	XMFLOAT3 scale;   //スケール
	XMFLOAT4 color;   //色

	float speed;       //速度
	XMFLOAT3 speedVec; //速度を加える方向
	float accel;       //加速度
	XMFLOAT3 accelVec; //加速度を加える方向

	OBB collision; //当たり判定
private:
	int playerObject; //プレイヤーの頂点データ
	int playerTex;    //プレイヤーのテクスチャ

	float jumpPower;      //ジャンプパワー
	float jumpPowerDecay; //ジャンプパワーの減衰量

	XMFLOAT3 totalSpeed; //速度を全て足し合わせたもの
	XMFLOAT3 totalAccel; //加速度を全て足し合わせたもの

};
