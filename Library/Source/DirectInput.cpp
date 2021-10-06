#include "./Header/DirectInput.h"

IDirectInput8* DirectInput::dinput = nullptr;
IDirectInputDevice8* DirectInput::devkeyborad = nullptr;
BYTE DirectInput::key[256] = {};

DirectInput* DirectInput::GetInstance()
{
	// �C���X�^���X�̐���
	static DirectInput instance;

	return &instance;
}

int DirectInput::InputInit(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(hr))
	{
#ifdef _DEBUG
			OutputDebugStringA("DirectInput8�̍쐬�Ɏ��s���܂���\n");
#endif // _DEBUG

		return -1;
	}

	// �L�[�{�[�h�f�o�C�X�̐���
	hr = dinput->CreateDevice(GUID_SysKeyboard, &devkeyborad, NULL);
	if (FAILED(hr))
	{
		return -1;
	}

	// ���̓f�[�^�`���̃Z�b�g
	hr = devkeyborad->SetDataFormat(&c_dfDIKeyboard); //�W���`��
	if (FAILED(hr))
	{
		return -1;
	}

	// �r�����䃌�x���̃Z�b�g
	hr = devkeyborad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(hr))
	{
		return -1;
	}

	return 0;
}

bool DirectInput::CheckHitKey(int keyCode)
{
	if (keyCode < 0 || keyCode >= sizeof(key))
	{
		return false;
	}

	if (KeyInputUpdate() != 0)
	{
		return false;
	}

	if (key[keyCode])
	{
		return true;
	}

	return false;
}

int DirectInput::CheckHitKeyAll(int CheckType)
{
	if ((CheckType & CHECKINPUT_KEY) != 0)
	{
		for (int i = 0; i < sizeof(key) / sizeof(key[0]); i++)
		{
			if (CheckHitKey(i) == true)
			{
				return 0;
			}
		}
	}
	if ((CheckType & CHECKINPUT_PAD) != 0)
	{

	}
	if ((CheckType & CHECKINPUT_MOUSE) != 0)
	{

	}

	return -1;
}

int DirectInput::GetHitKeyStateAll(char* KeyStateArray)
{
	if (KeyStateArray == nullptr)
	{
		return -1;
	}

	for (int i = 0; i < sizeof(key) / sizeof(key[0]); i++)
	{
		if (CheckHitKey(i) == true)
		{
			KeyStateArray[i] = (char)true;
		}
		else
		{
			KeyStateArray[i] = (char)false;
		}
	}

	return 0;
}

int DirectInput::KeyInputUpdate()
{
	HRESULT hr;

	ZeroMemory(key, sizeof(key));
	hr = devkeyborad->GetDeviceState(sizeof(key), key);
	if (FAILED(hr)) {
		// ���s�Ȃ�ĊJ�����Ă�����x�擾
		devkeyborad->Acquire();
		devkeyborad->GetDeviceState(sizeof(key), key);
	}

	return 0;
}
