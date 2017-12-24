//===============================================================================================================
//
// explosion.cpp
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
#include "debugproc.h"
#include "camera.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"

//============================================================================
//
// �}�N����`
//
//============================================================================

#define MAX_TEX_PATTERN    ( TEX_PATTERN_X * TEX_PATTERN_Y )
#define ANIME_INTERVAL     ( 5 )      // frame

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexExplosion( void );		// �����̒��_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

EXPLOSION g_aExplosion[ EXPLOSION_NUM ];

static int g_nAnimeNo;
static int g_nAnimeTime;

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	�����G�t�F�N�g�̐ݒ�
//	 S_OK	: �����G�t�F�N�g�̐ݒ�ɐ���
//	 E_FAIL	: �����G�t�F�N�g�̐ݒ�Ɏ��s
//
//============================================================================

HRESULT InitExplosion( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	g_nAnimeNo   = 0;
	g_nAnimeTime = 0;

	// �����G�t�F�N�g�̈ʒu�i�����A�傫���j������
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// �����G�t�F�N�g�̈ʒu
		g_aExplosion[ nLoop ].pos = D3DXVECTOR3( 0.0f, BILLBOARD_SIZE + 30.0f, 0.0f );
		
		// �����G�t�F�N�g�̌���
		g_aExplosion[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		
		// �����G�t�F�N�g�̑傫��
		g_aExplosion[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		g_aExplosion[ nLoop ].bUse = false;

	}

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice, 
									EXPLOSION_FILE,
									&g_aExplosion[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

	// ���_���̐ݒ�
	hr = MakeVertexExplosion();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// ���_���̐ݒ�Ɏ��s
	}

	return S_OK;
}

//============================================================================
//
// ���_���̍쐬 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	���_���̐ݒ�
//	 S_OK	: ���_���̐ݒ�ɐ���
//	 E_FAIL	: ���_���̐ݒ�Ɏ��s
//�@
//============================================================================

HRESULT MakeVertexExplosion( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// ���_�o�b�t�@�̐���
	//------------------------------

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// ���_�o�b�t�@�̍쐬
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aExplosion[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// �����G�t�F�N�g�p�̒��_�ݒ�
		g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

		pVtx[0].vtx = D3DXVECTOR3( -EXPLOSION_SIZE,  EXPLOSION_SIZE, 0.f );
		pVtx[1].vtx = D3DXVECTOR3(  EXPLOSION_SIZE,  EXPLOSION_SIZE, 0.f );
		pVtx[2].vtx = D3DXVECTOR3( -EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.f );
		pVtx[3].vtx = D3DXVECTOR3(  EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.f );

		for( int nVtx = 0; nVtx < 4; nVtx++ )
		{
			pVtx[ nVtx ].nor = D3DXVECTOR3( 0, 1, 0 );
			pVtx[ nVtx ].diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
	}

	//------------------------------
	
	return S_OK;
}

//============================================================================
//
// �X�V���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UpdataExplosion( void )
{
	g_nAnimeTime++;

	if( g_nAnimeTime > ANIME_INTERVAL )
	{
		if( g_nAnimeNo >= MAX_TEX_PATTERN )
		{
			for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
			{
				if( g_aExplosion[ nLoop ].bUse == true )
				{
					g_aExplosion[ nLoop ].bUse = false;
					g_nAnimeNo = 0;
				}
			}
		}

		SetTextureAnime( g_nAnimeNo );
		g_nAnimeNo++;
		g_nAnimeTime = 0;
	}
}

//============================================================================
//
// �`�揈�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void DrawExplosion( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �J�����̏����擾
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == true )
		{
			//------------------------------
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_aExplosion[ nLoop ].mtxWorld );

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_aExplosion[ nLoop ].scl.x, g_aExplosion[ nLoop ].scl.y, g_aExplosion[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aExplosion[ nLoop ].mtxWorld, &g_aExplosion[ nLoop ].mtxWorld, &mtxScl );

			// ��]�𔽉f
			// �t�s������߂�
			D3DXMatrixInverse( &g_aExplosion[ nLoop ].mtxWorld,	// ���[���h�}�g���b�N�X
							   NULL,
							   &Camera.mtxView );				// �r���[�}�g���b�N�X

			g_aExplosion[ nLoop ].mtxWorld._41 = 0.0f;
			g_aExplosion[ nLoop ].mtxWorld._42 = 0.0f;
			g_aExplosion[ nLoop ].mtxWorld._43 = 0.0f;

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTrans, g_aExplosion[ nLoop ].pos.x, g_aExplosion[ nLoop ].pos.y, g_aExplosion[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aExplosion[ nLoop ].mtxWorld, &g_aExplosion[ nLoop ].mtxWorld, &mtxTrans );

			// ���[���h�}�g���b�N�X�̐ݒ�
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aExplosion[ nLoop ].mtxWorld );


			//--------------------
			// �|���S���`�揈��
			//--------------------

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// ���_�t�H�[�}�b�g
			D3DDevice -> SetStreamSource( 0,
										  g_aExplosion[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// �e�N�X�`���̐ݒ�
			D3DDevice -> SetTexture( 0, g_aExplosion[ 0 ].pD3DTexture );

			// �|���S���̕`��
			//--------------------

			D3DDevice ->SetRenderState(D3DRS_ZENABLE, false);
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// �A���t�@�e�X�g��L���ɂ���
			D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// ��r����A���t�@�l�i 0�`255 �j
			D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// ��r����

			
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			D3DDevice ->SetRenderState(D3DRS_ZENABLE, true);
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// �A���t�@�e�X�g�𖳌��ɂ���

			//--------------------
		}
	}
}

//============================================================================
//
// �J������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UnInitExplosion( void )
{
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// �t�B�[���h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_aExplosion[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �t�B�[���h�̃e�N�X�`���̏I������
		if( g_aExplosion[ nLoop ].pD3DTexture != NULL )
		{
			g_aExplosion[ nLoop ].pD3DTexture -> Release();
			g_aExplosion[ nLoop ].pD3DTexture = NULL;
		}
	}
}

//============================================================================
//
// �e�N�X�`�����W���A�A�j���ԍ��ɂ���Č��肷�鏈�� �֐�
// �����F
//�@int animeNo
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

//----------------------------------------------------------------
//  �e�N�X�`�����W���A�A�j���ԍ��ɂ���Č���
//        animeNo   0 �` (�@MAX_TEX_PATTERN - 1�@)

void SetTextureAnime( int animeNo )
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	int animeNoX, animeNoY;
	float top, bottom, left, right;

	// �e�N�X�`���[�S�̂̏c�����Ԗڂ̃e�N�X�`����
	animeNoX = animeNo % TEX_PATTERN_X;
	animeNoY = animeNo / TEX_PATTERN_X;

	// �e�N�X�`�����W�̒l�ɕϊ�
	top    = ( 1.0f * ( animeNoY )     ) / TEX_PATTERN_Y;
	bottom = ( 1.0f * ( animeNoY + 1 ) ) / TEX_PATTERN_Y;
	left   = ( 1.0f * ( animeNoX )     ) / TEX_PATTERN_X;
	right  = ( 1.0f * ( animeNoX + 1 ) ) / TEX_PATTERN_X;

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == true )
		{
			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

			// �e�N�X�`���[���W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2( left, top );

			pVtx[1].tex = D3DXVECTOR2( right, top );

			pVtx[2].tex = D3DXVECTOR2( left, bottom );

			pVtx[3].tex = D3DXVECTOR2( right, bottom );

			// ���_�f�[�^���A�����b�N����
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
		}
	}
}

//============================================================================
//
// �����G�t�F�N�g���擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_aExplosion
//
//============================================================================

EXPLOSION* GetExplosion( void )
{
	return g_aExplosion;
}

//============================================================================
//
// �����G�t�F�N�g���擾 �֐�
// �����F
//�@D3DXVECTOR3 pos
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void SetExplosion( D3DXVECTOR3 pos )
{
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == false )
		{
			g_aExplosion[ nLoop ].pos.x = pos.x;
			g_aExplosion[ nLoop ].pos.z = pos.z;
			g_aExplosion[ nLoop ].bUse = true;
			break;
		}
	}
}

/* EOF */