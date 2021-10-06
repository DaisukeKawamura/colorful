#pragma once
#include "LoadTex.h"

class DebugText : public LoadTex
{
	/*ƒƒ“ƒo•Ï”*/
public:
	static const int fontWidth = 8;      //•¶š‚Ì‰¡•
	static const int fontHeight = 16;    //•¶š‚Ìc•
	static const int fontLineCount = 16; //ˆês‚ ‚½‚è‚Ì•¶š”
private:
	static UINT64 fontTexWidth;
	static UINT fontTexHeight;
	vector<IndexData> fontIndex;
	int charCount;

	/*ƒƒ“ƒoŠÖ”*/
public:
	DebugText(const DirectXInit* w);
	~DebugText();

	HRESULT DrawString(const float& posX, const float& posY, const float& fontScale,
		const XMFLOAT4& color, const char* text, ...);

	void TextLoopEnd();
private:
	HRESULT DrawStringInit();
};
