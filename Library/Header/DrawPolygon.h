#pragma once
#include "DebugText.h"
#include <./DirectXTex/d3dx12.h>

#define MAIN_CAMERA (0)

// マテリアル
struct Material
{
	std::string name;            //マテリアル名
	DirectX::XMFLOAT3 ambient;   //アンビエント影響度
	DirectX::XMFLOAT3 diffuse;   //ディフューズ影響度
	DirectX::XMFLOAT3 specular;  //スペキュラー影響度
	float alpha;                 //アルファ
	std::string textureFilename; //テクスチャファイル名

	// コンストラクタ
	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};

// objファイルのデータ
struct OBJDate : public Object
{
	/*配列にして使うことが前提*/
	/*先にモデル不要のオブジェクトを生成する*/

	Material material;
	size_t textrueIndex;

	// コンストラクタ
	OBJDate() : Object()
	{
		material = {};
		textrueIndex = -1;
	}
};

class DrawPolygon final : public DebugText
{
public: // メンバ関数
	// コンストラクタ
	DrawPolygon(const DirectXInit* w);
	// デストラクタ
	~DrawPolygon() {}

	// 三角形の作成
	int CreateTriangle(const XMFLOAT3& vertex1, const DirectX::XMFLOAT2& uv1,
		const XMFLOAT3& vertex2, const DirectX::XMFLOAT2& uv2,
		const XMFLOAT3& vertex3, const DirectX::XMFLOAT2& uv3, const bool& isFill = true);
	// 矩形の作成
	int CreateRect(const float& width, const float& height, const bool& isFill = true);
	// 正多角形の作成
	int CreateCircle(const float& r, const size_t& divNum, const bool& isFill = true);
	// 直方体の作成
	int Create3Dbox(const float& width, const float& height, const float& depth, const bool& isFill = true);
	// 正多角錐の作成
	int CreateCorn(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// 正多角柱の作成
	int CreateCylinder(const float& r, const float& h, const size_t& divNum, const bool& isFill = true);
	// 球体の作成
	int CreateSphere(const float& r, const size_t& divNum, const bool& isFill = true);

	// objファイルによるモデルの作成（filePath：objファイルの名前、directoryPath：モデルのファイルがあるフォルダ）
	int CreateOBJModel(const char* filePath, const char* directoryPath);

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

	// スクリーン座標からワールド座標を求める
	DirectX::XMFLOAT3 ScreenToWorld(int x, int y, float z);
	// ビュー行列の逆行列を取得（移動量は0固定）
	DirectX::XMMATRIX InverseView();

	// 内部の初期化用関数
	void PolygonLoopEnd();
private:
	// 描画関数の初期化
	int DrawPolygonInit();

	// マテリアル読み込み
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/*頂点座標とインデックスデータ計算用*/
	void Circle(const XMFLOAT3& centerPos, const float& r, const size_t& divNum, const bool& flag,
		Vertex* v, unsigned short* index);

private: // メンバ変数
	int polygonCount; //オブジェクトの数
	size_t loopCount; //ループした回数

	XMFLOAT3 lightVec; //光線
	XMFLOAT3 light;    //光源

	vector<XMMATRIX> matView; //ビュー変換行列(カメラ)
	int cameraNo;             //カメラの番号（最初はMAIN_CAMERAを指している）

	vector<OBJDate> objDate; //OBJファイルのデータ
};
