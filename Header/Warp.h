#pragma once
#include "./Header/DrawPolygon.h"
#include "./Header/LoadCSV.h"
#include <DirectXMath.h>
#include <vector>

class Warp
{
private: //エイリアス
	// DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;

public: //メンバ関数
	Warp();
	~Warp() {}

	// 初期化
	void Init(DrawPolygon* draw);
	// ワープの初期化
	void WarpInit(const int map[MAP_HEIGHT][MAP_WIDTH], const int& stageNo);
	// ワープ
	const XMFLOAT3& WarpPos(const int& warpNo);
	// 描画
	void Draw(const XMFLOAT3& offset);

private: //メンバ変数
	DrawPolygon* draw; //描画用
	int warpModel;

	std::vector<XMFLOAT3> warpPos; //ワープの出入口があるマップチップ座標
	bool warpFlag[10]; //ワープに入れるかどうか
	int warpExit[10];  //ワープの出口の座標インデックス
};
