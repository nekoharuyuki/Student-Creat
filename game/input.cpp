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

#include "DirectX3D.h"
#include "input.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// DirectInput�̕ϐ�
static LPDIRECTINPUT8 g_pDInput = NULL;			// DirectInput
static LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;	// DirectInput�f�o�C�X

static BYTE g_Key[ 256 ] = {NULL};				// ���͏����󂯂Ƃ�z��
static BYTE g_oldKey[ 256 ] = {NULL};			// ���͏����󂯂Ƃ�z��

//============================================================================
//
// DirectInput �������֐�
// �����F
//�@hWnd    �@�@�@�c  �E�C���h�E�̃n���h��
//�@hInstance     �c�@���̃v���O���������ʂ��邽�߂̃n���h��
// �߂�l�F
//�@DirectInput �������̏�������
//
//============================================================================

HRESULT InitDInput( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;

	// DirectInput�̍쐬
	hr = DirectInput8Create( hInstance,				// �C���X�^���X�n���h��
							 DIRECTINPUT_VERSION, 
							 IID_IDirectInput8,
							 ( void** )&g_pDInput,	// DirectInput�I�u�W�F�N�g���󂯂Ƃ�|�C���^�ϐ�
							 NULL ); 
	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// DirectInput9�I�u�W�F�N�g�̍쐬�Ɏ��s
	}

	// �f�o�C�X�E�I�u�W�F�N�g���쐬
	// �L�[�{�[�h�p�f�o�C�X�I�u�W�F�N�g�𐶐�
	hr = g_pDInput -> CreateDevice( GUID_SysKeyboard,
									&g_pDIDevice,
									NULL ); 
	if( FAILED( hr ) ) 
	{
	    return E_FAIL;	// DirectInputDevice9�I�u�W�F�N�g�̍쐬�Ɏ��s
	}

	// �f�[�^�`����ݒ�
	hr = g_pDIDevice -> SetDataFormat( &c_dfDIKeyboard );
	if( FAILED( hr ) )
	{
		return E_FAIL;	// �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s
	}

	//���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevice -> SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		return E_FAIL;	// �t�H�A�O���E���h����r�����[�h�̐ݒ�Ɏ��s
	}

	// �L�[�{�[�h�̃A�N�Z�X�����擾
	g_pDIDevice -> Acquire();

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

void UpdataDInput( void )
{
	// �ߋ��̃L�[�ۑ�
	for( int nLoop = 0; nLoop < 256; nLoop++ )
	{
		g_oldKey[ nLoop ] = g_Key[ nLoop ];
	}

	HRESULT hr;

	// �f�o�C�X�̒��ڃf�[�^���擾����
	hr = g_pDIDevice -> GetDeviceState( sizeof( g_Key ), &g_Key[ 0 ] );

	if( SUCCEEDED( hr ) )
	{
	}
	else
	{
		// �L�[�{�[�h�̃A�N�Z�X�����擾
		g_pDIDevice ->Acquire();
	}
}

//============================================================================
//
// �L�[���͂̔��� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�����Ă���Ƃ��@�c�@ture�@/�@�����Ă��Ȃ��Ƃ��@�c�@false
//
//============================================================================

bool nowKey( BYTE cKey )
{
	if( g_Key[ cKey ] && 0x80 )
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

bool PushKey( BYTE cKey )
{
	if( ( ( g_Key[ cKey ] ^ g_oldKey[ cKey ] ) &  g_Key[ cKey ] ) & 0x80 )
	{	// �������u��
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

bool ReleaseKey( BYTE cKey )
{
	if( ( ( g_Key[ cKey ] ^ g_oldKey[ cKey ] ) & ~g_Key[ cKey ] ) & 0x80 )
	{	// �������u��
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

void ReleaseDInput( void )
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

/* EOF */