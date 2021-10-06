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
	/*namespace���ȗ�*/
protected:
	template<class T>
	using vector = std::vector<T>;
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/*�����o�ϐ�*/
public:
	float clearColor[4]; //�w�i�F

	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
protected:
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;

	vector<ComPtr<IDXGIAdapter1>> adapters; //�A�_�v�^�[�̗񋓗p
	ComPtr<IDXGIAdapter1> tmpAdapter;       //�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������

	D3D_FEATURE_LEVEL levels[4]; //�Ή����x���̔z��
	D3D_FEATURE_LEVEL featureLevel;

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc;     //�W���ݒ�ŃR�}���h�L���[�𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc;       //�e��ݒ�����ăX���b�v�`�F�C���𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;       //�e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	vector<ComPtr<ID3D12Resource>> backBuffer; //���\��2�ɂ���

	// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;
private:
	int windowWidth, windowHeight; //��ʃT�C�Y

	CD3DX12_RESOURCE_DESC depthResDesc;     //���\�[�X�ݒ�
	ComPtr<ID3D12Resource> depthBuffer;     //���\�[�X����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc; //�[�x�r���[�p�ݒ�\����
	ComPtr<ID3D12DescriptorHeap> dsvHeap;   //�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;  //�[�x�r���[�쐬

	SIZE_T bbIndex; //�o�b�N�o�b�t�@�̃C���f�b�N�X

	/*�����o�֐�*/
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

	// �[�x�o�b�t�@�̐���
	HRESULT CreateDepthBuffer();
};
