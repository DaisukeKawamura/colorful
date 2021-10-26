#pragma once
#include <DirectXMath.h>
#include "./Header/DrawPolygon.h"
#include "./Header/OBBCollision.h"
#include <Header/LoadCSV.h>
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
private:
	static const XMFLOAT4 changeColor[4];

public: // メンバ関数
	// コンストラクタ
	Player();
	// デストラクタ
	~Player() {}

	// 初期化
	void Init(DrawPolygon* draw);
	// 更新
	void Update();
	// 描画
	void Draw();

	// プレイヤーのオブジェクトデータを取得 (戻り値が-1で失敗)
	int GetPlayerObject(const int& color) const;
	//色のブロックを踏んだ時の処理
	void ChangeGroundColor(const int map);
	/// <summary>
	/// ジャンプ開始時のパワーの設定
	/// </summary>
	/// <param name="jumpPower"> ジャンプパワー </param>
	/// <param name="jumpPowerDecay"> ジャンプパワーの減衰量 </param>
	void JumpStart(float jumpPower, float jumpPowerDecay);

public: // メンバ変数
	DrawPolygon* draw;  //描画用

	XMFLOAT3 pos;     //座標
	XMFLOAT3 oldPos;  //1つ前の座標
	XMMATRIX rotaMat; //回転行列
	XMFLOAT3 scale;   //スケール
	XMFLOAT4 color;   //色

	float speed;       //速度
	XMFLOAT3 speedVec; //速度を加える方向
	float accel;       //加速度
	XMFLOAT3 accelVec; //加速度を加える方向

	bool jumpFlag; //ジャンプフラグ
	int  jumpCount; //ジャンプカウント
	bool groundFlag; //地面に接したか
	int  groundColor; //どの色に接したか
	OBB collision; //当たり判定

	float cameraPosX;//カメラの位置
private:
	int playerObject[4]; //プレイヤーのオブジェクトデータ

	float jumpPower;      //ジャンプパワー
	float jumpPowerDecay; //ジャンプパワーの減衰量

	XMFLOAT3 totalSpeed; //速度を全て足し合わせたもの
	XMFLOAT3 totalAccel; //加速度を全て足し合わせたもの

};
//色情報
enum GroundColor
{
	red = 1,
	blue,
	green,
	yellow
};