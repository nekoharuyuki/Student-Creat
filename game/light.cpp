//===============================================================================================================
//
// field.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "main.h"
#include "directX3d.h"
#include "light.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static D3DLIGHT9 g_aLight[ 3 ];		// ���C�g���

//============================================================================
//
// ���C�g�̏����� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

/* Direct3D �ŗ��p�ł��郉�C�g�ɂ́A���� 3 ��ނ�����B */

//	�|�C���g ���C�g				�c�@D3DLIGHT_POINT			1
//	�f�B���N�V���i�� ���C�g		�c�@D3DLIGHT_SPOT			2
//	�X�|�b�g���C�g				�c�@D3DLIGHT_DIRECTIONAL	3

void InitLight( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ���C�g�̌���
	D3DXVECTOR3 vecDir[ 3 ];

	// ���C�g�̍쐬(��)
	ZeroMemory( &g_aLight[ 0 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 0 ].Type = D3DLIGHT_DIRECTIONAL;					// ���C�g�̎��
	g_aLight[ 0 ].Diffuse = D3DXCOLOR( 0.9f, 0.8f, 0.8f, 1.f );	// �g�U���ݒ� ( R, G, B, A )
	vecDir[ 0 ] = D3DXVECTOR3( -0.5f, -0.8f, 0.3f );			// ���C�g�̕���

	// ���C�g�̍쐬(��)
	ZeroMemory( &g_aLight[ 1 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 1 ].Type = D3DLIGHT_DIRECTIONAL;					// ���C�g�̎��
	g_aLight[ 1 ].Diffuse = D3DXCOLOR( 0.7f, 0.5f, 0.4f, 1.f );	// �g�U���ݒ� ( R, G, B, A )
	vecDir[ 1 ] = D3DXVECTOR3( 0.2f, 0.5f, 0.5f );				// ���C�g�̕���

	// ���C�g�̍쐬(��)
	ZeroMemory( &g_aLight[ 2 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 2 ].Type = D3DLIGHT_DIRECTIONAL;					// ���C�g�̎��
	g_aLight[ 2 ].Diffuse = D3DXCOLOR( 0.5f, 0.3f, 0.2f, 1.f );	// �g�U���ݒ� ( R, G, B, A )
	vecDir[ 2 ] = D3DXVECTOR3( 0.3f, -0.5f, -0.5f );			// ���C�g�̕���

	// ���C�g��ݒ�
	for( int nLoop = 0; nLoop < 3; nLoop++ )
	{

		D3DXVec3Normalize( ( D3DXVECTOR3* ) &g_aLight[ nLoop ].Direction, &vecDir[ nLoop ] );
		D3DDevice -> SetLight( nLoop, &g_aLight[ nLoop ] );
		D3DDevice -> LightEnable( nLoop, TRUE );
	}

	D3DDevice -> SetRenderState( D3DRS_LIGHTING, TRUE );
}

//============================================================================
//
// ���C�g�̍X�V �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UpdataLight( void )
{
	;
}

//============================================================================
//
// ���C�g�̕`�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void DrawLight( void )
{
	;
}

//============================================================================
//
// ���C�g�̏I������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UnInitLight( void )
{
	;
}

/* EOF */