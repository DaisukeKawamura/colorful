#include "./Header/DebugText.h"

/*static変数の初期化*/
UINT64 DebugText::fontTexWidth = {};
UINT DebugText::fontTexHeight = {};

DebugText::DebugText(const DirectXInit* w) :
	LoadTex(w),
	fontIndex{},
	charCount(-1)
{
}

DebugText::~DebugText()
{
}

HRESULT DebugText::DrawStringInit()
{
//#ifndef _DEBUG
//	return S_OK;
//#endif // !_DEBUG

	HRESULT hr = S_FALSE;
	static bool isInit = false;

	if (isInit == true)
	{
		return S_OK;
	}
	else
	{
		isInit = true;
		fontIndex.push_back({});

		fontIndex[0].constant = CreateSprite();
		if (fontIndex[0].constant == -1)
		{
			return S_FALSE;
		}

		fontIndex[0].textrue = LoadTextrue(L"./lib/DebugTextFont.png");
		if (fontIndex[0].textrue == -1)
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
		}

		// テクスチャデータ取得
		D3D12_RESOURCE_DESC resDesc = textrueData[fontIndex[0].textrue].texbuff->GetDesc();

		fontTexWidth = resDesc.Width;
		fontTexHeight = resDesc.Height;

		return Init();
	}
}

HRESULT DebugText::DrawString(const float& posX, const float& posY,
	const float& fontScale, const XMFLOAT4& color, const char* text, ...)
{
//#ifndef _DEBUG
//	return S_OK;
//#endif // !_DEBUG

	using namespace DirectX;

	HRESULT hr = S_FALSE;
	static bool isDrawString = false;
	static const char* string = nullptr;
	static const char* numberString = (char*)malloc(sizeof(char) * 21);
	static char character = '\0';
	static bool formatFlag = false;
	va_list args;
	va_start(args, text);

	if (isDrawString == false)
	{
		isDrawString = true;

		hr = DrawStringInit();
		if (FAILED(hr))
		{
			return hr;
		}
	}

#pragma region GraphicsCommand

	BaseDrawSpriteGraphics();

	XMFLOAT2 pixel = { 1.0f * fontScale / fontTexWidth, 1.0f * fontScale / fontTexHeight };

	// デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { texCommonData.descHeap.Get() };
	w->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// シェーダーリソースビューをセット
	w->cmdList->SetGraphicsRootDescriptorTable(1, textrueData[fontIndex[0].textrue].gpuDescHandle);

	for (size_t i = 0, j = 0, k = 0; text[j] != '\0'; i++)
	{
		bool isInit = false;

		if (text[j] == '%')
		{
			formatFlag = true;
		}

		if (formatFlag == true)
		{
			if (text[j + 1] == '\0')
			{
				break;
			}
			if (text[j + 1] == '%')
			{
				j++;
				character = '%';
				formatFlag = false;
			}
			else if (text[j + 1] == 'c')
			{
				j++;
				character = va_arg(args, char);
				formatFlag = false;
			}
			else
			{
				if (string == nullptr)
				{
					k = 0;
					int tempI;
					float tempF;

					switch (text[j + 1])
					{
					case 'd':
						tempI = va_arg(args, int);
						string = numberString;
						sprintf_s((char*)string, 21, "%d", tempI);
						break;
					case 'f':
						tempF = va_arg(args, float);
						string = numberString;
						sprintf_s((char*)string, 21, "%f", tempF);
						break;
					case 's':
						string = va_arg(args, char*);
						break;
					default:
						break;
					}
				}

				if (string[k] == '\0')
				{
					j += 2;
					string = nullptr;
					formatFlag = false;
					continue;
				}
				else
				{
					character = string[k];
				}
			}
		}
		else
		{
			character = text[j];
		}

		if ((size_t)(charCount + 1) < fontIndex.size())
		{
			isInit = true;
		}

		if (isInit == false)
		{
			fontIndex.push_back({ CreateSprite(), fontIndex[0].textrue });
			if (fontIndex[fontIndex.size() - 1].constant == -1)
			{
				return S_FALSE;
			}
		}

		charCount++;

		sprite[fontIndex[charCount].constant].size = { fontWidth * fontScale, fontHeight * fontScale };
		sprite[fontIndex[charCount].constant].texLeftTop.x = (character % fontLineCount) * (float)fontWidth;
		sprite[fontIndex[charCount].constant].texLeftTop.y = (character / fontLineCount) * (float)fontHeight;
		sprite[fontIndex[charCount].constant].texSize = { fontWidth, fontHeight };

		enum Corner { LB, LT, RB, RT };

		SpriteVertex vert[4];

		float left = 0.0f;
		float right = sprite[fontIndex[charCount].constant].size.x;
		float top = 0.0f;
		float bottom = sprite[fontIndex[charCount].constant].size.y;

		vert[LB].pos = { left,  bottom, 0.0f };
		vert[LT].pos = { left,     top, 0.0f };
		vert[RB].pos = { right, bottom, 0.0f };
		vert[RT].pos = { right,    top, 0.0f };

		float texLeft =
			sprite[fontIndex[charCount].constant].texLeftTop.x / fontTexWidth;
		float texRight =
			(sprite[fontIndex[charCount].constant].texLeftTop.x + sprite[fontIndex[charCount].constant].texSize.x - 1) / fontTexWidth;
		float texTop =
			sprite[fontIndex[charCount].constant].texLeftTop.y / fontTexHeight;
		float texBottom =
			(sprite[fontIndex[charCount].constant].texLeftTop.y + sprite[fontIndex[charCount].constant].texSize.y - 1) / fontTexHeight;

		vert[LB].uv = { texLeft,  texBottom };
		vert[LT].uv = { texLeft,     texTop };
		vert[RB].uv = { texRight, texBottom };
		vert[RT].uv = { texRight,    texTop };

		// 頂点バッファへのデータ転送
		SpriteVertex* vertexMap = nullptr;
		hr = sprite[fontIndex[charCount].constant].vertBuff->Map(0, nullptr, (void**)&vertexMap);
		memcpy(vertexMap, vert, sizeof(vert));
		sprite[fontIndex[charCount].constant].vertBuff->Unmap(0, nullptr);

		sprite[fontIndex[charCount].constant].color = color;
		sprite[fontIndex[charCount].constant].pos = { posX + fontWidth * fontScale * i + pixel.x, posY + pixel.y, 0 };
		sprite[fontIndex[charCount].constant].rotation = 0;

		sprite[fontIndex[charCount].constant].matWorld = XMMatrixIdentity();
		sprite[fontIndex[charCount].constant].matWorld *=
			XMMatrixRotationZ(XMConvertToRadians(sprite[fontIndex[charCount].constant].rotation));
		sprite[fontIndex[charCount].constant].matWorld *= XMMatrixTranslation(
			sprite[fontIndex[charCount].constant].pos.x,
			sprite[fontIndex[charCount].constant].pos.y,
			sprite[fontIndex[charCount].constant].pos.z);

		SpriteConstBufferData* constMap = nullptr;
		hr = sprite[fontIndex[charCount].constant].constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->color = sprite[fontIndex[charCount].constant].color;
		constMap->mat = sprite[fontIndex[charCount].constant].matWorld *
			spriteData.matProjection[CommonData::Projection::ORTHOGRAPHIC];
		sprite[fontIndex[charCount].constant].constBuff->Unmap(0, nullptr);

		// 定数バッファビューをセット
		w->cmdList->SetGraphicsRootConstantBufferView(0, sprite[fontIndex[charCount].constant].constBuff->GetGPUVirtualAddress());

		// 頂点バッファの設定
		w->cmdList->IASetVertexBuffers(0, 1, &sprite[fontIndex[charCount].constant].vbView);
		// 描画コマンド
		w->cmdList->DrawInstanced(4, 1, 0, 0);

		if (formatFlag == true)
		{
			k++;
		}
		else
		{
			j++;
		}
	}

#pragma endregion

	va_end(args);
	return hr;
}

void DebugText::TextLoopEnd()
{
	LoopEnd();

	charCount = -1;
}
