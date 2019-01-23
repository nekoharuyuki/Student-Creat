//===============================================================================================================
//
// input.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "main.h"
#include "DirectX3D.h"
#include "debugproc.h"
#include "input.h"
#include "mouse.h"

#include "model.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// DirectInput�̕ϐ�
static LPDIRECTINPUT8 g_pDInput = NULL;			// DirectInput
static LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;	// DirectInput�f�o�C�X
#define DIDEVICE_BUFFERSIZE	100					// �f�o�C�X�ɐݒ肷��o�b�t�@�E�T�C�Y

static DIMOUSESTATE2 g_Mouse;		// ���͏����󂯂Ƃ�
static DIMOUSESTATE2 g_OldMouse;	// ���͏����󂯂Ƃ�
static MOUSE MouseState;			// �}�E�X�|�C���^���

//============================================================================
//
// DirectMouse �������֐�
// �����F
//�@hWnd    �@�@�@�c  �E�C���h�E�̃n���h��
//�@hInstance     �c�@���̃v���O���������ʂ��邽�߂̃n���h��
// �߂�l�F
//�@DirectMouse �������̏�������
//
//============================================================================

HRESULT InitDMouse( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;

	// �}�E�X�|�C���^���̏�����
	MouseState.x = SCREEN_WIDTH  / 2;									// �}�E�X�J�[�\���̂w���W��������
	MouseState.y = SCREEN_HEIGHT / 2;									// �}�E�X�J�[�\���̂x���W��������
	MouseState.lButton = false;							// ���{�^���̏���������
	MouseState.rButton = false;							// �E�{�^���̏���������
	MouseState.cButton = false;							// �����{�^���̏���������

	// DirectInput�̍쐬
	hr = DirectInput8Create( hInstance,				// �C���X�^���X�n���h��
							 DIRECTINPUT_VERSION, 
							 IID_IDirectInput8,
							 ( void** )&g_pDInput,	// DirectInput�I�u�W�F�N�g���󂯂Ƃ�|�C���^�ϐ�
							 NULL ); 
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
		return E_FAIL;	// DirectInput9�I�u�W�F�N�g�̍쐬�Ɏ��s
	}

	// �f�o�C�X�E�I�u�W�F�N�g���쐬
	// �L�[�{�[�h�p�f�o�C�X�I�u�W�F�N�g�𐶐�
	hr = g_pDInput -> CreateDevice( GUID_SysMouse,
									&g_pDIDevice,
									NULL ); 
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
	    return E_FAIL;	// DirectInputDevice9�I�u�W�F�N�g�̍쐬�Ɏ��s
	}

	// �f�[�^�`����ݒ�
	hr = g_pDIDevice -> SetDataFormat(&c_dfDIMouse2);
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
	    return E_FAIL;	// �f�[�^�`���̐ݒ�Ɏ��s
	}

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevice -> SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		MouseState.bUse = false;
		return E_FAIL;	// �t�H�A�O���E���h����r�����[�h�̐ݒ�Ɏ��s
	}

	// �����[�h��ݒ�i���Βl���[�h�ɐݒ�j
	DIPROPDWORD diprop;
	diprop.diph.dwSize	     = sizeof(diprop); 
	diprop.diph.dwHeaderSize = sizeof(diprop.diph); 
	diprop.diph.dwObj	     = 0;
	diprop.diph.dwHow	     = DIPH_DEVICE;
	diprop.dwData		     = DIPROPAXISMODE_REL;
//	diprop.dwData		     = DIPROPAXISMODE_ABS;	// ��Βl���[�h�̏ꍇ

	hr = g_pDIDevice -> SetProperty( DIPROP_AXISMODE, &diprop.diph );

	if( FAILED( hr ) )
	{
		MouseState.bUse = false;
		return E_FAIL;	// �����[�h�̐ݒ�Ɏ��s
	}

	// �L�[�{�[�h�̃A�N�Z�X�����擾
	g_pDIDevice -> Acquire();
	MouseState.bUse = true;

	return S_OK;
}

//============================================================================
//
// DirectInput�̍X�V �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UpdataDMouse( HWND hWnd )
{
	// �ߋ��̃L�[�ۑ�
	for( int nLoop = 0; nLoop < 8; nLoop++ )
	{
		g_OldMouse.rgbButtons[nLoop] = g_Mouse.rgbButtons[nLoop];
	}

	HRESULT hr;

	// �f�o�C�X�̒��ڃf�[�^���擾����
	hr = g_pDIDevice -> GetDeviceState( sizeof( DIMOUSESTATE2 ), &g_Mouse );

	if( SUCCEEDED( hr ) )
	{
		MouseState.bUse = true;
	}
	else
	{
		MouseState.bUse = false;

		// �}�E�X�̃A�N�Z�X�����擾
		g_pDIDevice->Acquire();
	}

	POINT pos = {0};

	// �X�N���[�����W���擾
	GetCursorPos(&pos);

	// �N���C�A���g�̈�ɕϊ�
	ScreenToClient(hWnd, &pos);

	// �擾�����������Ƀ}�E�X�̏����X�V
	// �X�N���[���̒��S���i 0, 0 �j
	MouseState.x = (float)pos.x - SCREEN_WIDTH  / 2;
	MouseState.y = (float)-pos.y + SCREEN_HEIGHT / 2;

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O���\��
	PrintDebugProc("Mouse POS : ( %.2f : %.2f )\n", MouseState.x, MouseState.y);

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

}

//============================================================================
//
// �L�[���͂̔��� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//  �Ȃ�
//
//�@�����Ă���Ƃ��@�c�@ture�@/�@�����Ă��Ȃ��Ƃ��@�c�@false
//
//============================================================================

bool nowClick( int nClick )
{
	(g_Mouse.rgbButtons[nClick] & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
	{	// �����Ă���
		// ����
		return true;
	}
	else
	{	// �����Ă��Ȃ�
		return false;
	}
}

//============================================================================
//
// �L�[���͂̔��� �֐�(�������u��)
// �����F
//�@�Ȃ�
// �߂�l�F
//  �Ȃ�
//�@�������u�ԁ@�c�@ture�@/�@�����Ă��Ȃ��Ƃ��@�c�@false
//
//============================================================================

bool PushClick( int nClick )
{
	( ( ( g_Mouse.rgbButtons[nClick] ^ g_OldMouse.rgbButtons[nClick] ) & g_Mouse.rgbButtons[nClick] ) & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
	{	// �����Ă���
		// ����
		return true;
	}
	else
	{	// �����Ă��Ȃ�
		return false;
	}
}

//============================================================================
//
// �L�[���͂̔��� �֐�(�������u��)
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�������u�ԁ@�c�@ture�@/�@�����Ă��Ȃ��Ƃ��@�c�@false
//
//============================================================================

bool ReleaseClick( int nClick )
{
	( ( ( g_Mouse.rgbButtons[nClick] ^ g_OldMouse.rgbButtons[nClick] ) & ~g_Mouse.rgbButtons[nClick] ) & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
	{	// �����Ă���
		// ����
		return true;
	}
	else
	{	// �����Ă��Ȃ�
		return false;
	}
}

//============================================================================
//
// DirectInput�f�o�C�X�ƃI�u�W�F�N�g�̊J�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void ReleaseDMouse( void )
{
	if( NULL != g_pDIDevice )
	{	// �f�o�C�X�̊J��
		g_pDIDevice -> Unacquire();
		g_pDIDevice -> Release();
		g_pDIDevice = NULL;
	}

	if( NULL != g_pDInput )
	{	// �I�u�W�F�N�g�̊J��
		g_pDInput -> Release();
		g_pDInput = NULL;
	}
}

//============================================================================
//
// DirectInput�f�o�C�X�̏��擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

MOUSE GetMouseState(void)
{
	return MouseState;
}

/* EOF */