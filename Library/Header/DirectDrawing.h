#pragma once
#include "DirectXInit.h"
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <./DirectXTex/d3dx12.h>

/*�V�F�[�_�p*/
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

/*DirectDrawing�̃o�[�W�����w��*/
#define DIRECTDRAWING_VERSION 0x0001

/*�u�����h���[�h*/
#define BLENDMODE_NOBLEND (0) //�m�[�u�����h
#define BLENDMODE_ALPHA   (1) //���u�����h
#define BLENDMODE_ADD     (2) //���Z����
#define BLENDMODE_SUB     (3) //���Z����
#define BLENDMODE_INV     (4) //�F���]����

// ���_�f�[�^�\����
struct Vertex
{
	DirectX::XMFLOAT3 pos;    //xyz���W
	DirectX::XMFLOAT3 normal; //�@���x�N�g��
	DirectX::XMFLOAT2 uv;     //uv���W
};

struct VertexData
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	std::vector<Vertex> vertices;    //���_�f�[�^
	ComPtr<ID3D12Resource> vertBuff; //���_�o�b�t�@�̐���
	D3D12_VERTEX_BUFFER_VIEW vbView; //���_�o�b�t�@�r���[�̍쐬

	std::vector<unsigned short> indices; //�C���f�b�N�X�f�[�^
	ComPtr<ID3D12Resource> indexBuff;    //�C���f�b�N�X�o�b�t�@�̐ݒ�
	D3D12_INDEX_BUFFER_VIEW ibView;      //�C���f�b�N�X�o�b�t�@�r���[�̍쐬
};

// ���_�f�[�^�\����(�X�v���C�g�p)
struct SpriteVertex
{
	DirectX::XMFLOAT3 pos;    //xyz���W
	DirectX::XMFLOAT2 uv;     //uv���W
};

// �萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferData
{
	DirectX::XMFLOAT4 color;    //�F (RGBA)
	DirectX::XMMATRIX mat;      //3D�ϊ��s��
	DirectX::XMFLOAT3 lightVec; //�����ւ̃x�N�g��
};
// �萔�o�b�t�@�p�f�[�^�\����(�X�v���C�g�p)
struct SpriteConstBufferData
{
	DirectX::XMFLOAT4 color;    //�F (RGBA)
	DirectX::XMMATRIX mat;      //3D�ϊ��s��
};

// �f�B�X�N���v�^�n���h��
struct DescHandle
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandle; //CPU���̃n���h��
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle; //GPU���̃n���h��
};

// �e�I�u�W�F�N�g(�X�v���C�g)�ŋ��ʂ̃f�[�^���܂Ƃ߂��\����
struct CommonData
{
	/*'matProjection'�̈���*/
	enum Projection
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate; //�p�C�v���C���̏��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature; //���[�g�V�O�l�`��

	DirectX::XMMATRIX matProjection[2] = {};

	CommonData();
};

struct IndexData
{
	int constant;
	int textrue;
};

// �I�u�W�F�N�g�̃��[���h�s����܂Ƃ߂��\����
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

// �X�v���C�g�ꖇ���̃f�[�^���܂Ƃ߂��\����
struct Sprite
{
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;  //���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView;                  //���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff; //�萔�o�b�t�@

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
	/*namespace���ȗ�*/
protected:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
	template<class T>
	using vector = std::vector<T>;
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	/*�����o�ϐ�*/
protected:
	const DirectXInit* w;

	vector<VertexData> vertices; //���_�f�[�^
	Vertex* vertMap;

	vector<Object> obj;
	vector<IndexData> objIndex;
	ComPtr<ID3D12DescriptorHeap> basicDescHeap; //�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v
	vector<DescHandle> srvHandle;
	size_t nullBufferCount;

	static vector<Sprite> sprite;
	static vector<IndexData> spriteIndex;

	ComPtr<ID3DBlob> vsBlob;    //���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;    //�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; //�G���[�I�u�W�F�N�g
	ComPtr<ID3DBlob> rootSigBlob;

	CommonData objectData;
	static CommonData spriteData;
private:
	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spritePipeline;
	D3D12_RENDER_TARGET_BLEND_DESC& blendDesc;
	D3D12_RENDER_TARGET_BLEND_DESC& spriteBlendDesc;

	float nearClip; //�j�A�N���b�v����
	float farClip;  //�t�@�[�N���b�v����

	/*�����o�֐�*/
public:
	DirectDrawing(const DirectXInit* w);
	~DirectDrawing();

	// ��̒萔�o�b�t�@�̐���
	int CreateNullConstant(const XMFLOAT3& pos, const XMMATRIX& rota, const XMFLOAT3& scale);
	// �萔�o�b�t�@�̍X�V
	void UpdataConstant(const XMFLOAT3& pos, const XMMATRIX& rota, const XMFLOAT3& scale,
		const int& objectIndex, const int& polygonData = -1, const int& parent = -1);
	int SetDrawBlendMode(int blendMode);
	HRESULT SetNearFar(float nearClip, float farClip);
protected:
	HRESULT Init();
	HRESULT DrawingInit();
	HRESULT SpriteDrawingInit();
	// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̐���
	int CreateVertexAndIndexBuffer();
	// �萔�o�b�t�@�̐���
	HRESULT CreateConstBuffer(int* objIndex);
	// �X�v���C�g�̐���
	int CreateSprite();
	HRESULT BasicDescHeapInit();

	// �`�揈���̋��ʕ���
	void BaseDrawGraphics();
	// �`�揈���̋��ʕ���(�X�v���C�g�p)
	void BaseDrawSpriteGraphics();
};
