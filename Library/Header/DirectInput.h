#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// CheckHitKeyAll �Œ��ׂ���̓^�C�v
#define CHECKINPUT_KEY   (0x01) //�L�[���͂𒲂ׂ�
#define CHECKINPUT_PAD   (0x02) //�p�b�h���͂𒲂ׂ�
#define CHECKINPUT_MOUSE (0x04) //�}�E�X�{�^�����͂𒲂ׂ�
#define CHECKINPUT_ALL   (CHECKINPUT_KEY | CHECKINPUT_PAD | CHECKINPUT_MOUSE) //���ׂĂ̓��͂𒲂ׂ�

//namespace DirectInput
//{
//	int InputInit(HINSTANCE hInstance, HWND hWnd);
//	bool CheckHitKey(int keyCode);
//	int CheckHitKeyAll(int CheckType = CHECKINPUT_ALL);
//	int GetHitKeyStateAll(char* KeyStateArray);
//}

//using namespace DirectInput;

class DirectInput final
{
private:
	// �R���X�g���N�^��private�ɂ���
	DirectInput() {};
	// �f�X�g���N�^��private�ɂ���
	~DirectInput() {};
public:
	// �R�s�[�R���X�g���N�^�𖳌��ɂ���
	DirectInput(const DirectInput& obj) = delete;
	// ������Z�q�𖳌��ɂ���
	const DirectInput& operator=(const DirectInput& obj) = delete;
	// �C���X�^���X�̎擾
	static DirectInput* GetInstance();

public: // �ÓI�����o�ϐ�
	static BYTE key[256];
private:
	static IDirectInput8* dinput;
	static IDirectInputDevice8* devkeyborad;

public: //�����o�֐�
	// ���̓f�o�C�X�̏�����
	int InputInit(HINSTANCE hInstance, HWND hWnd);
	// ����̃L�[�{�[�h�̓��͏�Ԃ̎擾
	bool CheckHitKey(int keyCode);
	// �S�Ă̓��̓f�o�C�X�̏�Ԃ̎擾
	int CheckHitKeyAll(int CheckType = CHECKINPUT_ALL);
	// �L�[�{�[�h�f�o�C�X�̓��͏�Ԃ̎擾
	int GetHitKeyStateAll(char* KeyStateArray);
private:
	// �L�[�{�[�h�f�o�C�X�̍X�V
	int KeyInputUpdate();

};
