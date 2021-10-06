#pragma once
#include "LoadTex.h"

class DebugText : public LoadTex
{
	/*�����o�ϐ�*/
public:
	static const int fontWidth = 8;      //�����̉���
	static const int fontHeight = 16;    //�����̏c��
	static const int fontLineCount = 16; //��s������̕�����
private:
	static UINT64 fontTexWidth;
	static UINT fontTexHeight;
	vector<IndexData> fontIndex;
	int charCount;

	/*�����o�֐�*/
public:
	DebugText(const DirectXInit* w);
	~DebugText();

	HRESULT DrawString(const float& posX, const float& posY, const float& fontScale,
		const XMFLOAT4& color, const char* text, ...);

	void TextLoopEnd();
private:
	HRESULT DrawStringInit();
};
