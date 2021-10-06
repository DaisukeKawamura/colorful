#pragma once
#include "DirectXInit.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <./DirectXTex/d3dx12.h>

/*シェーダ用*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

/*DirectDrawingのバージョン指定*/
#define DIRECTDRAWING_VERSION 0x0001

/*ブレンドモード*/
#define BLENDMODE_NOBLEND (0) //ノーブレンド
#define BLENDMODE_ALPHA   (1) //αブレンド
#define BLENDMODE_ADD     (2) //加算合成
#define BLENDMODE_SUB     (3) //減算合成
#define BLENDMODE_INV     (4) //色反転合成

// 頂点データ構造体
struct Vertex
{
	DirectX::XMFLOAT3 pos;    //xyz座標
	DirectX::XMFLOAT3 normal; //法線ベクトル
	DirectX::XMFLOAT2 uv;     //uv座標
};

struct VertexData
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	std::vector<Vertex> vertices;    //頂点データ
	ComPtr<ID3D12Resource> vertBuff; //頂点バッファの生成
	D3D12_VERTEX_BUFFER_VIEW vbView; //頂点バッファビューの作成

	std::vector<unsigned short> indices; //インデックスデータ
	ComPtr<ID3D12Resource> indexBuff;    //インデックスバッファの設定
	D3D12_INDEX_BUFFER_VIEW ibView;      //インデックスバッファビューの作成
};

// 頂点データ構造体(スプライト用)
struct SpriteVertex
{
	DirectX::XMFLOAT3 pos;    //xyz座標
	DirectX::XMFLOAT2 uv;     //uv座標
};

// 定数バッファ用データ構造体
struct ConstBufferData
{
	DirectX::XMFLOAT4 color;    //色 (RGBA)
	DirectX::XMMATRIX mat;      //3D変換行列
	DirectX::XMFLOAT3 lightVec; //光源へのベクトル
};
// 定数バッファ用データ構造体(スプライト用)
struct SpriteConstBufferData
{
	DirectX::XMFLOAT4 color;    //色 (RGBA)
	DirectX::XMMATRIX mat;      //3D変換行列
};

// ディスクリプタハンドル
struct DescHandle
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle; //CPU側のハンドル
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle; //GPU側のハンドル
};

// 各オブジェクト(スプライト)で共通のデータをまとめた構造体
struct CommonData
{
	/*'matProjection'の引数*/
	enum Projection
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate; //パイプラインの状態
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature; //ルートシグネチャ

	DirectX::XMMATRIX matProjection[2] = {};

	CommonData();
};

struct IndexData
{
	int constant;
	int textrue;
};

// オブジェクトのワールド行列をまとめた構造体
struct Object
{
	int polygonData = -1;
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;

	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMMATRIX rotation = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

	int parent = -1;
};

// スプライト一枚分のデータをまとめた構造体
struct Sprite
{
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;  //頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vbView;                  //頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; //定数バッファ

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };

	DirectX::XMFLOAT3 pos = { 0, 0, 0 };
	float rotation = 0.0f;
	DirectX::XMFLOAT2 size = { 0.000000001f, 0.000000001f };

	DirectX::XMFLOAT2 anchorpoint = { 0.5f, 0.5f };

	DirectX::XMFLOAT2 texLeftTop = { 0, 0 };
	DirectX::XMFLOAT2 texSize = { 100, 100 };
};

class DirectDrawing
{
	/*namespace名省略*/
protected:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	template<class T>
	using vector = std::vector<T>;
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/*メンバ変数*/
protected:
	const DirectXInit* w;

	vector<VertexData> vertices; //頂点データ
	Vertex* vertMap;

	vector<Object> obj;
	vector<IndexData> objIndex;
	ComPtr<ID3D12DescriptorHeap> basicDescHeap; //定数バッファ用のデスクリプタヒープ
	vector<DescHandle> srvHandle;
	size_t nullBufferCount;

	static vector<Sprite> sprite;
	static vector<IndexData> spriteIndex;

	ComPtr<ID3DBlob> vsBlob;    //頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    //ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; //エラーオブジェクト
	ComPtr<ID3DBlob> rootSigBlob;

	CommonData objectData;
	static CommonData spriteData;
private:
	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spritePipeline;
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc;
	D3D12_RENDER_TARGET_BLEND_DESC& spriteBlendDesc;

	float nearClip; //ニアクリップ距離
	float farClip;  //ファークリップ距離

	/*メンバ関数*/
public:
	DirectDrawing(const DirectXInit* w);
	~DirectDrawing();

	// 空の定数バッファの生成
	int CreateNullConstant(const XMFLOAT3& pos, const XMMATRIX& rota, const XMFLOAT3& scale);
	// 定数バッファの更新
	void UpdataConstant(const XMFLOAT3& pos, const XMMATRIX& rota, const XMFLOAT3& scale,
		const int& objectIndex, const int& polygonData = -1, const int& parent = -1);
	int SetDrawBlendMode(int blendMode);
	HRESULT SetNearFar(float nearClip, float farClip);
protected:
	HRESULT Init();
	HRESULT DrawingInit();
	HRESULT SpriteDrawingInit();
	// 頂点バッファとインデックスバッファの生成
	int CreateVertexAndIndexBuffer();
	// 定数バッファの生成
	HRESULT CreateConstBuffer(int* objIndex);
	// スプライトの生成
	int CreateSprite();
	HRESULT BasicDescHeapInit();

	// 描画処理の共通部分
	void BaseDrawGraphics();
	// 描画処理の共通部分(スプライト用)
	void BaseDrawSpriteGraphics();
};
