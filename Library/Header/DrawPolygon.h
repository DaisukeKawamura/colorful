#pragma once
#include "DebugText.h"
#include <./DirectXTex/d3dx12.h>

#define MAIN_CAMERA (0)

class DrawPolygon final : public DebugText
{
	/*メンバ変数*/
private:
	int polygonCount;
	size_t loopCount;

	ConstBufferData* constMap;
	XMMATRIX mat;
	XMFLOAT3 lightVec;
	XMFLOAT3 light;

	vector<XMMATRIX> matView;  //ビュー変換行列(カメラ)
	int cameraNo;

	/*メンバ関数*/
public:
	DrawPolygon(const DirectXInit* w);
	~DrawPolygon() {};

	// 三角形の作成
	int CreateTriangle(const XMFLOAT3& vertex1, const DirectX::XMFLOAT2& uv1,
		const XMFLOAT3& vertex2, const DirectX::XMFLOAT2& uv2,
		const XMFLOAT3& vertex3, const DirectX::XMFLOAT2& uv3, const bool& isFill = true);
	// 矩形の作成
	int CreateRect(const float& width, const float& height, const bool& isFill = true);
	// 正多角形の作成
	int CreateCircle(const float& r, const size_t& divNum, const bool& isFill = true);
	// 正多角錐の作成
	int CreateCorn(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// 正多角柱の作成
	int CreateCylinder(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// 球体の描画処理
	int CreateSphere(const float& r, const size_t& divNum, const bool& isFill = true);

	// テクスチャのタイリング
	void NormalizeUV(const int& polygonData, const int& graphHandle);
	// テクスチャの移動
	void MoveUV(const int& polygonData, const int& graphHandle, DirectX::XMFLOAT2 speed);

	// オブジェクトの描画処理
	int Draw(const int& polygonData, const XMFLOAT3& position, const XMMATRIX& rotation, const XMFLOAT3& scale,
		const XMFLOAT4& color, const int& graphHandle = 0, const bool& isFill = true,
		const bool& isOrthoGraphic = false, const bool& viewMultiFlag = true, const int& parent = -1);

	// カメラの作成
	int CreateCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector);
	// カメラの設定
	void SetCamera(const XMFLOAT3& cameraPos, const XMFLOAT3& cameraTarget, const XMFLOAT3& upVector,
		const int& cameraNo = MAIN_CAMERA);
	// カメラの切り替え
	void ChangeCamera(const int& cameraNo);

	DirectX::XMFLOAT3 InverseWorld(int x, int y, float z);
	DirectX::XMMATRIX InverseView();

	// 内部の初期化用関数
	void PolygonLoopEnd();
private:
	int DrawPolygonInit();

	/*頂点座標とインデックスデータ計算用*/
	void Circle(const XMFLOAT3& centerPos, const float& r, const size_t& divNum, const bool& flag,
		Vertex* v, unsigned short* index);

};
