#include "./Header/DirectXInit.h"
#include "./Header/DrawPolygon.h"
#include "./Header/DirectInput.h"
#include "./Header/SafeDelete.h"

/*�E�B���h�E�T�C�Y*/
const int window_width = 1280; //����
const int window_height = 720; //�c��

const WCHAR title[] = L"title"; //�^�C�g��

const float clearColor[] = { 0.1f,0.1f,0.1f,0.0f }; //�w�i�F

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	using namespace DirectX;

	DirectXInit w{};

	w.title = title;
	for (UINT i = 0; i < 4; i++)
	{
		w.clearColor[i] = clearColor[i];
	}
	w.SetWindowSize(window_width, window_height);
	w.Init();

	DrawPolygon draw(&w);

	DirectInput* input = DirectInput::GetInstance();
	input->InputInit(w.w.hInstance, w.hWnd);

	// �摜�̓ǂݍ���
	int background = draw.LoadTextrue(L"./Resources/background.png");
	int tex1 = draw.LoadTextrue(L"./Resources/tex1.png");

	XMFLOAT3 cameraPos = { 0, 0, -100 }; //�J�����̈ʒu
	XMFLOAT3 cameraTarget = { 0, 0, 0 }; //�J�����̒����_
	XMFLOAT3 upVec = { 0, 1, 0 }; //��x�N�g��

	// ���C���J�����̏�����
	draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);

	// �I�u�W�F�N�g�̐���
	int triangle = draw.CreateTriangle(
		XMFLOAT3(-10.0f, -10.0f, 0.0f), XMFLOAT2(0.0f, 1.0f),
		XMFLOAT3(  0.0f, +10.0f, 0.0f), XMFLOAT2(0.5f, 0.0f),
		XMFLOAT3(+10.0f, -10.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),
		true);
	int cylinder = draw.CreateCylinder(10.0f, 10.0f, 8, true);

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	XMFLOAT3 cylinderPos = { 0, 0, 0 }; //�ʒu

	char keys[256] = {};
	char oldkeys[256] = {};

	while (true)
	{
		if (w.WindowMessage() == -1) { break; }

		for (size_t i = 0; i < 256; i++)
		{
			oldkeys[i] = keys[i];
		}
		input->GetHitKeyStateAll(keys);

		/* �X�V */

		if (keys[DIK_LEFT])
		{
			cylinderPos.x -= 1.0f;
		}
		if (keys[DIK_RIGHT])
		{
			cylinderPos.x += 1.0f;
		}
		if (keys[DIK_UP])
		{
			cylinderPos.y += 1.0f;
		}
		if (keys[DIK_DOWN])
		{
			cylinderPos.y -= 1.0f;
		}

		if (keys[DIK_A])
		{
			cameraPos.x -= 1.0f;

			// ���C���J�����̍X�V
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_D])
		{
			cameraPos.x += 1.0f;

			// ���C���J�����̍X�V
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_W])
		{
			cameraPos.y += 1.0f;

			// ���C���J�����̍X�V
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}
		if (keys[DIK_S])
		{
			cameraPos.y -= 1.0f;

			// ���C���J�����̍X�V
			draw.SetCamera(cameraPos, cameraTarget, upVec, MAIN_CAMERA);
		}

		w.ClearScreen();
		draw.SetDrawBlendMode(BLENDMODE_ALPHA);

		/* �`�� */

		// �w�i
		draw.DrawTextrue(0, 0, window_width, window_height, 0, background, XMFLOAT2(0.0f, 0.0f));

		// 3d�I�u�W�F�N�g
		draw.Draw(
			triangle,
			XMFLOAT3(10.0f, 10.0f, 10.0f),
			XMMatrixRotationY(XMConvertToRadians(75)),
			XMFLOAT3(1, 1, 1),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			tex1
		);
		draw.Draw(
			cylinder,
			cylinderPos,
			XMMatrixIdentity(),
			XMFLOAT3(1, 1, 1),
			XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		);

		// �O�i
		draw.DrawTextrue(
			window_width,
			window_height,
			100,
			100,
			0,
			tex1,
			XMFLOAT2(1.0f, 1.0f)
		);
		draw.DrawCutTextrue(
			0,
			window_height,
			100,
			100,
			XMFLOAT2(0, 320),
			XMFLOAT2(320, 480),
			0,
			background,
			XMFLOAT2(0.0f, 1.0f)
		);

		// ���[�v�̏I������
		draw.PolygonLoopEnd();
		w.ScreenFlip();

		if (input->CheckHitKey(DIK_ESCAPE)) { break; }
	}

	w.WindowEnd();

	return 0;
}