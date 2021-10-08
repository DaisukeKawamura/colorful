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
protected: // �G�C���A�X
	// std::���ȗ�
	template<class T> using vector = std::vector<T>;
	// Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	// �f�t�H���g�R���X�g���N�^
	DirectXInit();
#ifdef UNICODE
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="windowSize"> �E�B���h�E�T�C�Y </param>
	/// <param name="title"> �^�C�g�� </param>
	/// <param name="className"> �N���X�� </param>
	DirectXInit(RECT windowSize, LPCWSTR title, LPCWSTR className = L"DirectX");
#else
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="windowSize"> �E�B���h�E�T�C�Y </param>
	/// <param name="title"> �^�C�g�� </param>
	/// <param name="className"> �N���X�� </param>
	DirectXInit(RECT windowSize, LPCSTR title, LPCSTR className = "DirectX");
#endif // UNICODE
	// �f�X�g���N�^
	~DirectXInit();

	// ������
	HRESULT Init();

	// �`��O����
	void ClearScreen();
	// �`��㏈��
	void ScreenFlip();

	// �E�B���h�E�T�C�Y�̐ݒ�
	int SetWindowSize(int width, int height);

	// �E�B���h�E�̉����̎擾(�����^)
	LONG GetWindowWidth() const;
	// �E�B���h�E�̏c���̎擾(�����^)
	LONG GetWindowHeight() const;
	// �E�B���h�E�̉����̎擾(���������_�^)
	float GetWindowWidthF() const;
	// �E�B���h�E�̉����̎擾(���������_�^)
	float GetWindowHeightF() const;
private:
	// �f�o�b�O���C���[
	void DebugLayer();

	// �[�x�o�b�t�@�̐���
	HRESULT CreateDepthBuffer();

public: // �����o�ϐ�
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

};
