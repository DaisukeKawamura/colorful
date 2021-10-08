#include "./Header/LoadTex.h"

#define EoF -1 // End of Function

TextrueCommon::TextrueCommon() :
	metadata{},
	scratchImg{},
	img{},
	textrueCount(1),
	descHeap{}
{
	noneTextrue[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
}

/*static変数の初期化*/
DirectDrawing::vector<Textrue> LoadTex::textrueData = {};
TextrueCommon LoadTex::texCommonData = {};

LoadTex::LoadTex(const DirectXInit* w) :
	DirectDrawing(w),
	spriteCount(0)
{
}

int LoadTex::LoadTextrue(const wchar_t* fileName)
{
	HRESULT hr = S_FALSE;
	static bool isLoadTextrueInit = false;
	CD3DX12_RESOURCE_DESC texResDesc{};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; //設定構造体

	if (isLoadTextrueInit == false)
	{
		isLoadTextrueInit = true;

		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{}; //設定構造体
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; //シェーダから見える
		descHeapDesc.NumDescriptors = (UINT)(50000); //テクスチャバッファの数
		// デスクリプタヒープの生成
		hr = w->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&texCommonData.descHeap));
		if (FAILED(hr))
		{
			return EoF;
		}
	}

	if (textrueData.size() <= 0)
	{
		texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			1,
			1,
			1
		);

		textrueData.push_back({});
		hr = w->dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
			nullptr,
			IID_PPV_ARGS(&textrueData[0].texbuff)
		);
		if (FAILED(hr))
		{
			return EoF;
		}

		// テクスチャバッファにデータ転送
		hr = textrueData[0].texbuff->WriteToSubresource(
			0,
			nullptr,                   //全領域へコピー
			texCommonData.noneTextrue, //元データアドレス
			sizeof(XMFLOAT4) * 1,      //1ラインサイズ
			sizeof(XMFLOAT4) * 1       //全サイズ
		);
		if (FAILED(hr))
		{
			return EoF;
		}

		// デスクリプタヒープの先頭ハンドル(CPU)を取得
		textrueData[textrueData.size() - 1].cpuDescHandle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				texCommonData.descHeap->GetCPUDescriptorHandleForHeapStart(),
				(INT)(textrueData.size() - 1),
				w->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

		// デスクリプタヒープの先頭ハンドル(GPU)を取得
		textrueData[textrueData.size() - 1].gpuDescHandle =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				texCommonData.descHeap->GetGPUDescriptorHandleForHeapStart(),
				(INT)(textrueData.size() - 1),
				w->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

		// シェーダリソースビュー設定
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;

		w->dev->CreateShaderResourceView(
			textrueData[0].texbuff.Get(), //ビューと関連付けるバッファ
			&srvDesc, //テクスチャ設定情報
			textrueData[textrueData.size() - 1].cpuDescHandle
		);
	}
	if (fileName != nullptr)
	{
		texCommonData.textrueCount++;

		// WICテクスチャのロード
		LoadFromWICFile(
			fileName,
			DirectX::WIC_FLAGS_IGNORE_SRGB,
			&texCommonData.metadata,
			texCommonData.scratchImg
		);

		texCommonData.img = texCommonData.scratchImg.GetImage(0, 0, 0);

		texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
			texCommonData.metadata.format,
			texCommonData.metadata.width,
			(UINT)texCommonData.metadata.height,
			(UINT16)texCommonData.metadata.arraySize,
			(UINT16)texCommonData.metadata.mipLevels
		);

		textrueData.push_back({});
		hr = w->dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, //テクスチャ用指定
			nullptr,
			IID_PPV_ARGS(&textrueData[textrueData.size() - 1].texbuff)
		);
		if (FAILED(hr))
		{
			return EoF;
		}

		// テクスチャバッファにデータ転送
		hr = textrueData[textrueData.size() - 1].texbuff->WriteToSubresource(
			0,
			nullptr,                            //全領域へコピー
			texCommonData.img->pixels,          //元データアドレス
			(UINT)texCommonData.img->rowPitch,  //1ラインサイズ
			(UINT)texCommonData.img->slicePitch //一枚サイズ
		);
		if (FAILED(hr))
		{
			return EoF;
		}

		// デスクリプタヒープの先頭ハンドル(CPU)を取得
		textrueData[textrueData.size() - 1].cpuDescHandle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				texCommonData.descHeap->GetCPUDescriptorHandleForHeapStart(),
				(INT)(textrueData.size() - 1),
				w->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

		// デスクリプタヒープの先頭ハンドル(GPU)を取得
		textrueData[textrueData.size() - 1].gpuDescHandle =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				texCommonData.descHeap->GetGPUDescriptorHandleForHeapStart(),
				(INT)(textrueData.size() - 1),
				w->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

		// シェーダリソースビュー設定
		srvDesc.Format = texCommonData.metadata.format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; //2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;

		w->dev->CreateShaderResourceView(
			textrueData[textrueData.size() - 1].texbuff.Get(), //ビューと関連付けるバッファ
			&srvDesc, //テクスチャ設定情報
			textrueData[textrueData.size() - 1].cpuDescHandle
		);
	}

	return texCommonData.textrueCount - 1;
}

int LoadTex::DrawTextureInit()
{
	int size = 0;
	static bool isDrawTextrueInit = false;

	size = CreateSprite();
	if (size == EoF)
	{
		return EoF;
	}

	if (isDrawTextrueInit == false)
	{
		isDrawTextrueInit = true;

		if (LoadTextrue() == EoF)
		{
			return EoF;
		}

		if (FAILED(Init()))
		{
			return EoF;
		}
	}

	return size;
}

int LoadTex::DrawTextrue(const float& posX, const float& posY, const float& width, const float& height,
	const float& angle, const int& graphHandle, const DirectX::XMFLOAT2& anchorpoint,
	const XMFLOAT4& color, const int& parent)
{
	using namespace DirectX;

	if ((graphHandle < 0 || (UINT)graphHandle > texCommonData.textrueCount) ||
		(parent < -1 || (parent != -1 && (size_t)parent >= spriteIndex.size())))
	{
		return EoF;
	}

	bool isInit = false;

	if ((size_t)(spriteCount + 1) < spriteIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawTextureInit();
		if (size == EoF)
		{
			return EoF;
		}

		spriteIndex.push_back({ size, graphHandle });
	}

	if (spriteIndex.size() == 0)
	{
		return EoF;
	}

	spriteCount++;
	spriteCount %= spriteIndex.size();

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	if (sprite[spriteIndex[spriteCount].constant].size.x != width ||
		sprite[spriteIndex[spriteCount].constant].size.y != height ||
		sprite[spriteIndex[spriteCount].constant].anchorpoint.x != anchorpoint.x ||
		sprite[spriteIndex[spriteCount].constant].anchorpoint.y != anchorpoint.y)
	{
		sprite[spriteIndex[spriteCount].constant].size = { width, height };
		sprite[spriteIndex[spriteCount].constant].anchorpoint = anchorpoint;

		enum Corner { LB, LT, RB, RT };

		SpriteVertex vert[] = {
			{{}, { 0.0f, 1.0f }},
			{{}, { 0.0f, 0.0f }},
			{{}, { 1.0f, 1.0f }},
			{{}, { 1.0f, 0.0f }},
		};

		float left = (0.0f - sprite[spriteIndex[spriteCount].constant].anchorpoint.x) *
			sprite[spriteIndex[spriteCount].constant].size.x;
		float right = (1.0f - sprite[spriteIndex[spriteCount].constant].anchorpoint.x) *
			sprite[spriteIndex[spriteCount].constant].size.x;
		float top = (0.0f - sprite[spriteIndex[spriteCount].constant].anchorpoint.y) *
			sprite[spriteIndex[spriteCount].constant].size.y;
		float bottom = (1.0f - sprite[spriteIndex[spriteCount].constant].anchorpoint.y) *
			sprite[spriteIndex[spriteCount].constant].size.y;

		vert[LB].pos = {  left, bottom, 0.0f };
		vert[LT].pos = {  left,    top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right,    top, 0.0f };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		sprite[sprite.size() - 1].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[sprite.size() - 1].vertBuff->Unmap(0, nullptr);
	}

	sprite[spriteIndex[spriteCount].constant].color = color;
	sprite[spriteIndex[spriteCount].constant].pos = { posX, posY, 0 };
	sprite[spriteIndex[spriteCount].constant].rotation = angle;

	sprite[spriteIndex[spriteCount].constant].matWorld = XMMatrixIdentity();
	sprite[spriteIndex[spriteCount].constant].matWorld *=
		XMMatrixRotationZ(XMConvertToRadians(sprite[spriteIndex[spriteCount].constant].rotation));
	sprite[spriteIndex[spriteCount].constant].matWorld *=
		XMMatrixTranslation(
			sprite[spriteIndex[spriteCount].constant].pos.x,
			sprite[spriteIndex[spriteCount].constant].pos.y,
			sprite[spriteIndex[spriteCount].constant].pos.z);

	if (parent != -1)
	{
		sprite[spriteIndex[spriteCount].constant].matWorld *= sprite[parent].matWorld;
	}

	SpriteConstBufferData* constMap = nullptr;
	sprite[spriteIndex[spriteCount].constant].constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = sprite[spriteIndex[spriteCount].constant].color;
	constMap->mat = sprite[spriteIndex[spriteCount].constant].matWorld *
		spriteData.matProjection[CommonData::Projection::ORTHOGRAPHIC];
	sprite[spriteIndex[spriteCount].constant].constBuff->Unmap(0, nullptr);

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	w->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	w->cmdList->SetGraphicsRootConstantBufferView(0, sprite[spriteIndex[spriteCount].constant].constBuff->GetGPUVirtualAddress());
	w->cmdList->SetGraphicsRootDescriptorTable(1, textrueData[spriteIndex[spriteCount].textrue].gpuDescHandle);

	// 頂点バッファの設定
	w->cmdList->IASetVertexBuffers(0, 1, &sprite[spriteIndex[spriteCount].constant].vbView);
	// 描画コマンド
	w->cmdList->DrawInstanced(4, 1, 0, 0);

#pragma endregion

	return spriteIndex[spriteCount].constant;
}

int LoadTex::DrawCutTextrue(const float& posX, const float& posY, const float& width, const float& height,
	const DirectX::XMFLOAT2& texPos, const DirectX::XMFLOAT2& texSize, const float& angle,
	const int& graphHandle, const DirectX::XMFLOAT2& anchorpoint, const XMFLOAT4& color, const int& parent)
{
	using namespace DirectX;

	if ((graphHandle < 0 || (UINT)graphHandle > texCommonData.textrueCount) ||
		(parent < -1 && (size_t)parent >= spriteIndex.size()))
	{
		return EoF;
	}

	bool isInit = false;

	if ((size_t)(spriteCount + 1) < spriteIndex.size())
	{
		isInit = true;
	}

	if (isInit == false)
	{
		int size = DrawTextureInit();
		if (size == EoF)
		{
			return EoF;
		}

		spriteIndex.push_back({ size, graphHandle });
	}

	if (spriteIndex.size() == 0)
	{
		return EoF;
	}

	spriteCount++;
	spriteCount %= spriteIndex.size();

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	if (sprite[spriteCount].size.x != width ||
		sprite[spriteCount].size.y != height ||
		sprite[spriteCount].texLeftTop.x != texPos.x ||
		sprite[spriteCount].texLeftTop.y != texPos.y ||
		sprite[spriteCount].texSize.x != texSize.x ||
		sprite[spriteCount].texSize.y != texSize.y ||
		sprite[spriteIndex[spriteCount].constant].anchorpoint.x != anchorpoint.x ||
		sprite[spriteIndex[spriteCount].constant].anchorpoint.y != anchorpoint.y)
	{
		sprite[spriteCount].size = { width, height };
		sprite[spriteCount].texLeftTop = texPos;
		sprite[spriteCount].texSize = texSize;
		sprite[spriteCount].anchorpoint = anchorpoint;

		enum Corner { LB, LT, RB, RT };

		SpriteVertex vert[] = {
			{{}, { 0.0f, 1.0f }},
			{{}, { 0.0f, 0.0f }},
			{{}, { 1.0f, 1.0f }},
			{{}, { 1.0f, 0.0f }},
		};

		float left = (0.0f - sprite[spriteCount].anchorpoint.x) * sprite[spriteCount].size.x;
		float right = (1.0f - sprite[spriteCount].anchorpoint.x) * sprite[spriteCount].size.x;
		float top = (0.0f - sprite[spriteCount].anchorpoint.y) * sprite[spriteCount].size.y;
		float bottom = (1.0f - sprite[spriteCount].anchorpoint.y) * sprite[spriteCount].size.y;

		vert[LB].pos = {  left, bottom, 0.0f };
		vert[LT].pos = {  left,    top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right,    top, 0.0f };

		// テクスチャデータ取得
		D3D12_RESOURCE_DESC resDesc = textrueData[spriteIndex[spriteCount].textrue].texbuff->GetDesc();

		float texLeft = sprite[spriteCount].texLeftTop.x / resDesc.Width;
		float texRight = (sprite[spriteCount].texLeftTop.x + sprite[spriteCount].texSize.x) / resDesc.Width;
		float texTop = sprite[spriteCount].texLeftTop.y / resDesc.Height;
		float texBottom = (sprite[spriteCount].texLeftTop.y + sprite[spriteCount].texSize.y) / resDesc.Height;

		vert[LB].uv = {  texLeft, texBottom };
		vert[LT].uv = {  texLeft,    texTop };
		vert[RB].uv = { texRight, texBottom };
		vert[RT].uv = { texRight,    texTop };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		sprite[sprite.size() - 1].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[sprite.size() - 1].vertBuff->Unmap(0, nullptr);
	}

	sprite[spriteIndex[spriteCount].constant].color = color;
	sprite[spriteIndex[spriteCount].constant].pos = { posX, posY, 0 };
	sprite[spriteIndex[spriteCount].constant].rotation = angle;

	sprite[spriteIndex[spriteCount].constant].matWorld = XMMatrixIdentity();
	sprite[spriteIndex[spriteCount].constant].matWorld *=
		XMMatrixRotationZ(XMConvertToRadians(sprite[spriteIndex[spriteCount].constant].rotation));
	sprite[spriteIndex[spriteCount].constant].matWorld *=
		XMMatrixTranslation(
			sprite[spriteIndex[spriteCount].constant].pos.x,
			sprite[spriteIndex[spriteCount].constant].pos.y,
			sprite[spriteIndex[spriteCount].constant].pos.z);

	if (parent != -1)
	{
		sprite[spriteIndex[spriteCount].constant].matWorld *= sprite[parent].matWorld;
	}

	SpriteConstBufferData* constMap = nullptr;
	sprite[spriteIndex[spriteCount].constant].constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = sprite[spriteIndex[spriteCount].constant].color;
	constMap->mat = sprite[spriteIndex[spriteCount].constant].matWorld *
		spriteData.matProjection[CommonData::Projection::ORTHOGRAPHIC];
	sprite[spriteIndex[spriteCount].constant].constBuff->Unmap(0, nullptr);

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	w->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	w->cmdList->SetGraphicsRootConstantBufferView(0, sprite[spriteIndex[spriteCount].constant].constBuff->GetGPUVirtualAddress());
	w->cmdList->SetGraphicsRootDescriptorTable(1, textrueData[spriteIndex[spriteCount].textrue].gpuDescHandle);

	// 頂点バッファの設定
	w->cmdList->IASetVertexBuffers(0, 1, &sprite[spriteCount].vbView);
	// 描画コマンド
	w->cmdList->DrawInstanced(4, 1, 0, 0);

#pragma endregion

	return spriteIndex[spriteCount].constant;
}

void LoadTex::LoopEnd()
{
	spriteCount = -1;
}
