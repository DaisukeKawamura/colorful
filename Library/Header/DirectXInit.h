#pragma once
#include "Win32API.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include <./DirectXTex/d3dx12.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DirectXInit : public Win32API
{
	/*namespace名省略*/
protected:
	template<class T>
	using vector = std::vector<T>;
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/*メンバ変数*/
public:
	float clearColor[4]; //背景色

	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
protected:
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;

	vector<ComPtr<IDXGIAdapter1>> adapters; //アダプターの列挙用
	ComPtr<IDXGIAdapter1> tmpAdapter;       //ここに特定の名前を持つアダプターオブジェクトが入る

	D3D_FEATURE_LEVEL levels[4]; //対応レベルの配列
	D3D_FEATURE_LEVEL featureLevel;

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;     //標準設定でコマンドキューを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc;       //各種設定をしてスワップチェインを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;       //各種設定をしてデスクリプタヒープを生成
	vector<ComPtr<ID3D12Resource>> backBuffer; //裏表の2つについて

	// フェンスの生成
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;
private:
	int windowWidth, windowHeight; //画面サイズ

	CD3DX12_RESOURCE_DESC depthResDesc;     //リソース設定
	ComPtr<ID3D12Resource> depthBuffer;     //リソース生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc; //深度ビュー用設定構造体
	ComPtr<ID3D12DescriptorHeap> dsvHeap;   //深度ビュー用デスクリプタヒープ作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;  //深度ビュー作成

	SIZE_T bbIndex; //バックバッファのインデックス

	/*メンバ関数*/
public:
	DirectXInit();
#ifdef UNICODE
	DirectXInit(RECT windowSize, LPCWSTR title, LPCWSTR className = L"DirectX");
#else
	DirectXInit(RECT windowSize, LPCSTR title, LPCSTR className = "DirectX");
#endif // UNICODE
	~DirectXInit();

	HRESULT Init();

	void ClearScreen();
	void ScreenFlip();

	int SetWindowSize(int width, int height);

	LONG GetWindowWidth() const;
	LONG GetWindowHeight() const;
	float GetWindowWidthF() const;
	float GetWindowHeightF() const;
private:
	void DebugLayer();

	// 深度バッファの生成
	HRESULT CreateDepthBuffer();
};
