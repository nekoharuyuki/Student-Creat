//===============================================================================================================
//
// shadow.cpp
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
#include "shadow.h"
#include "model.h"
#include "billboard.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

SHADOW g_aShadow[ MAX_SHADOW ];

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	�t�B�[���h�̐ݒ�
//	 S_OK	: �t�B�[���h�̐ݒ�ɐ���
//	 E_FAIL	: �t�B�[���h�̐ݒ�Ɏ��s
//
//============================================================================

HRESULT InitShadow( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// �r���{�[�h�̉e�̈ʒu�i�����A�傫���j������
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		// �e�̈ʒu
		g_aShadow[ nLoop ].pos = D3DXVECTOR3( 0.0f, 0.05f, 0.0f );

		// �e�̌���
		g_aShadow[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		// �e�̑傫��
		g_aShadow[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		g_aShadow[ nLoop ].bUse = false;
	
		// �e�N�X�`���̓ǂݍ���
		hr = D3DXCreateTextureFromFile( D3DDevice, SHADOW_FILE, &g_aShadow[ nLoop ].pD3DTexture );
	}

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

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

void UpdataShadow( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	//------------------------------
	// ���_�o�b�t�@�̐���
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[nLoop].bUse == true )
		{
			// ���_�o�b�t�@�̍쐬
			D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											 D3DUSAGE_WRITEONLY,
											 FVF_VERTEX_3D,
											 D3DPOOL_MANAGED,
											 &g_aShadow[ nLoop ].pD3DVtxBtxBuffer,
											 NULL );

			// ���_���̐ݒ�
			VERTEX_3D *pVtx;

			// �e�p�̉e
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

			pVtx[0].vtx = D3DXVECTOR3( -g_aShadow[ nLoop ].fsize, 0.f,  g_aShadow[ nLoop ].fsize );
			pVtx[1].vtx = D3DXVECTOR3(  g_aShadow[ nLoop ].fsize, 0.f,  g_aShadow[ nLoop ].fsize );
			pVtx[2].vtx = D3DXVECTOR3( -g_aShadow[ nLoop ].fsize, 0.f, -g_aShadow[ nLoop ].fsize );
			pVtx[3].vtx = D3DXVECTOR3(  g_aShadow[ nLoop ].fsize, 0.f, -g_aShadow[ nLoop ].fsize );

			for( int nBillboardVtx = 0; nBillboardVtx < 4; nBillboardVtx++ )
			{
				pVtx[ nBillboardVtx ].nor = D3DXVECTOR3( 0, 1, 0 );
				pVtx[ nBillboardVtx ].diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			}

			pVtx[0].tex = D3DXVECTOR2( 0, 0 );
			pVtx[1].tex = D3DXVECTOR2( 1, 0 );
			pVtx[2].tex = D3DXVECTOR2( 0, 1 );
			pVtx[3].tex = D3DXVECTOR2( 1, 1 );

			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
		}
	}

	//------------------------------
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

void DrawShadow( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	//------------------------------
	// ���[���h�}�g���b�N�X�̐ݒ�
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[ nLoop ].bUse == true )
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_aShadow[ nLoop ].mtxWorld );

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_aShadow[ nLoop ].scl.x, g_aShadow[ nLoop ].scl.y, g_aShadow[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxScl );

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aShadow[ nLoop ].rot.y, g_aShadow[ nLoop ].rot.x, g_aShadow[ nLoop ].rot.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxRot );

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTrans, g_aShadow[ nLoop ].pos.x, g_aShadow[ nLoop ].pos.y, g_aShadow[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxTrans );

			// ���[���h�}�g���b�N�X�̐ݒ�
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aShadow[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// �|���S���`�揈��
			//--------------------

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// ���_�t�H�[�}�b�g
			D3DDevice -> SetStreamSource( 0,
										  g_aShadow[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// �e�N�X�`���̐ݒ�
			D3DDevice -> SetTexture( 0, g_aShadow[ nLoop ].pD3DTexture );

			// �e�̕`��ݒ�
			//--------------------

			D3DDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			D3DDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			D3DDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			// �`��ݒ�����ɖ߂�
			//--------------------

			D3DDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			D3DDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			D3DDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

void UnInitShadow( void )
{
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		// �t�B�[���h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_aShadow[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �t�B�[���h�̃e�N�X�`���̏I������
		if( g_aShadow[ nLoop ].pD3DTexture != NULL )
		{
			g_aShadow[ nLoop ].pD3DTexture -> Release();
			g_aShadow[ nLoop ].pD3DTexture = NULL;
		}
	}
}

int SetShadow( D3DXVECTOR3 pos, float size )
{
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[ nLoop ].bUse == false )
		{
			g_aShadow[ nLoop ].bUse = true;
			g_aShadow[ nLoop ].fsize = size;
			g_aShadow[ nLoop ].pos.x = pos.x;
			g_aShadow[ nLoop ].pos.z = pos.z;

			return nLoop;
		}
	}
	return -1;
}

void ResetShadow( int nNumber, D3DXVECTOR3 pos )
{
	g_aShadow[ nNumber ].pos.x = pos.x;
	g_aShadow[ nNumber ].pos.z = pos.z;
}

void DestroyShadow( int nNumber )
{
	g_aShadow[ nNumber ].bUse = false;
}

/* EOF */