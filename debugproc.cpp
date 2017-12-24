//===============================================================================================================
//
// debugproc.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "main.h"
#include "input.h"
#include "directX3d.h"
#include "debugproc.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static LPD3DXFONT	g_pD3DXFont = NULL;				// �t�H���g�I�u�W�F�N�g�|�C���^
static char g_aStrDebug[ MAX_FONT ] = {"\0"};		// �f�o�b�O���o�b�t�@
static bool g_bDrawTex = false;						// �`��t���O

//============================================================================
//
// �f�o�b�N��� ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@HRESULT hr
//
//============================================================================

HRESULT InitDebugProc( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �t�H���g�̐ݒ�
	HRESULT hr;
	hr = D3DXCreateFont( D3DDevice,
						 FONT_SIZE,					// �����T�C�Y
						 0,
						 0,
						 0,
						 FALSE, SHIFTJIS_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 DEFAULT_QUALITY,
						 DEFAULT_PITCH,
						 "Terminal",
						 &g_pD3DXFont );

	// �f�o�b�N���̃N���A
	for( int nLoop = 0; nLoop < MAX_FONT; nLoop++ )
	{
		g_aStrDebug[ nLoop ] = '\0';
	}

	// �`�悵�Ȃ��ݒ�
	g_bDrawTex = false;

	return hr;
}

//============================================================================
//
// �f�o�b�N���̕`�揈�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void DrawDebugProc(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	// �L�[���͔���
	if( PushKey( DIK_F1 ) )
	{
		if( g_bDrawTex == false )
		{	// �`�悷��
			g_bDrawTex = true;
		}
		else if( g_bDrawTex == true )
		{	// �`�悵�Ȃ�
			g_bDrawTex = false;
		}
	}

	if( g_bDrawTex == true )
	{	// �`�悷��
		g_pD3DXFont -> DrawText( NULL,
							 g_aStrDebug,
							 -1,
							 &rect,
							 DT_LEFT,
							 D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
	}
	
	// �f�o�b�O���o�b�t�@�̃N���A
	for( int nLoop = 0; nLoop < MAX_FONT; nLoop++ )
	{
		g_aStrDebug[ nLoop ] = '\0';
	}
}

//============================================================================
//
// �f�o�b�N���̍X�V���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UpdataDebugProc(void)
{
	;
}

//============================================================================
//
// �f�o�b�N���̊J������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UnInitDebugProc( void )
{
	// �t�H���g�I�u�W�F�N�g���
	if( NULL != g_pD3DXFont )
	{	// �f�o�C�X�̊J��
		g_pD3DXFont -> Release();
		g_pD3DXFont = NULL;
	}
}

//============================================================================
//
// �f�o�b�N���̍X�V���� �֐�
// �����F
//�@char *fmt, ...		�ψ���
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void PrintDebugProc( char *fmt, ... )
{
	char cTemp[ MAX_FONT ];
	va_list pArg;
	va_start( pArg, fmt );
	vsprintf_s( cTemp, sizeof( cTemp ), fmt, pArg );

	strcat_s( &g_aStrDebug[ 0 ], sizeof( g_aStrDebug ), &cTemp[ 0 ]);

	va_end( pArg );
}

//va_list				�ϒ������𑀍삷�邽�߂ɕK�v�ȍ\����
//va_start (ap, v)		�ϒ������𑀍삷���ŕK�v�ȃf�[�^���擾����}�N��
//va_arg (list, mode)	�ϒ���������A���Ɉ��������o���}�N��
//va_end (list)			�ϒ������̑�����I������}�N��

//============================================================================
//
// �f�o�b�N���`��t���O�̎擾 ���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_bDrawTex
//
//============================================================================

bool DebugProcFlg( void )
{
	return g_bDrawTex;
}