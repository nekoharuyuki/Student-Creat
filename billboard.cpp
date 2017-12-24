//===============================================================================================================
//
// billboard.cpp
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
#include "billboard.h"
#include "explosion.h"
#include "camera.h"
#include "model.h"
#include "meshfield.h"
#include "shadow.h"
#include "dungeon.h"

/* �T�E���h�Ɋւ���w�b�_�[�t�@�C�� */
#include "com.h"
#include "sound.h"
#include "snddata.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexBillboard( void );		// �e�̒��_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static BILLBOARD g_aBillboard[ BILLBOARD_NUM ];

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

HRESULT InitBillboard( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		// �e�̈ʒu�i�����A�傫���j������
		g_aBillboard[ nLoop ].pos = D3DXVECTOR3( 0.0f, BILLBOARD_SIZE + 30.0f, 0.0f );	// �ʒu
		g_aBillboard[ nLoop ].rot = D3DXVECTOR3( 0.0f, D3DXToRadian( 90 ), 0.0f );		// ����
		g_aBillboard[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );					// �傫��

		g_aBillboard[ nLoop ].bUse = false;

		g_aBillboard[ nLoop ].fShadowSize = SHADOW_SIZE_BILLBOARD;
	}

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice, BILLBOARD_FILE, &g_aBillboard[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

	// ���_���̐ݒ�
	hr = MakeVertexBillboard();

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

HRESULT MakeVertexBillboard( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		//------------------------------
		// ���_�o�b�t�@�̐���
		//------------------------------

		// ���_�o�b�t�@�̍쐬
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aBillboard[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

		pVtx[0].vtx = D3DXVECTOR3( -BILLBOARD_SIZE,  BILLBOARD_SIZE, 0.0f );
		pVtx[1].vtx = D3DXVECTOR3(  BILLBOARD_SIZE,  BILLBOARD_SIZE, 0.0f );
		pVtx[2].vtx = D3DXVECTOR3( -BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f );
		pVtx[3].vtx = D3DXVECTOR3(  BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f );

		for( int i = 0; i < 4; i++ )
		{
			pVtx[ i ].nor = D3DXVECTOR3( 0, 1, 0 );
			pVtx[ i ].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		pVtx[0].tex = D3DXVECTOR2( 0, 0 );
		pVtx[1].tex = D3DXVECTOR2( 1, 0 );
		pVtx[2].tex = D3DXVECTOR2( 0, 1 );
		pVtx[3].tex = D3DXVECTOR2( 1, 1 );

		g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------
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

void UpdataBillboard( void )
{
	//-------------------------
	// �e�̓����蔻�菈��
	//-------------------------

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if( g_aBillboard[ nLoop ].bUse == true )
		{
			// �ړ�����
			g_aBillboard[ nLoop ].pos.x += -sin( g_aBillboard[ nLoop ].Move.y ) * BILLBOARD_MOVE_SPEED;
			g_aBillboard[ nLoop ].pos.z += -cos( g_aBillboard[ nLoop ].Move.y ) * BILLBOARD_MOVE_SPEED;

			// �e�̏���
			ResetShadow(g_aBillboard[ nLoop ].nShadowNumber, g_aBillboard[ nLoop ].pos);

			if( g_aBillboard[ nLoop ].pos.z < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) )        ||
				g_aBillboard[ nLoop ].pos.z > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 100.f ||
				g_aBillboard[ nLoop ].pos.x < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) )        ||
				g_aBillboard[ nLoop ].pos.x > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) )
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE002 );
				
				// ������ݒ�
				SetExplosion( g_aBillboard[ nLoop ].pos );
				
				// �e�̏���
				g_aBillboard[ nLoop ].bUse = false;
				
				// �e�̏���
				DestroyShadow(g_aBillboard[ nLoop ].nShadowNumber);
				
				break;
			}

			int posX= (int)( g_aBillboard[ nLoop ].pos.x + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );
			int posZ= (int)( g_aBillboard[ nLoop ].pos.z + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );

			int Dungeon = GetDungeon( ( int )( posX ), ( int )( posZ ) );

			if( Dungeon == 1 )
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE002 );

				// ������ݒ�
				SetExplosion( g_aBillboard[ nLoop ].pos );

				// �e�̏���
				g_aBillboard[ nLoop ].bUse = false;

				// �e�̏���
				DestroyShadow(g_aBillboard[ nLoop ].nShadowNumber);
				break;
			}
		}
	}

	//-------------------------

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

void DrawBillboard( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �J�����̏����擾
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if(g_aBillboard[ nLoop ].bUse == true)
		{
			//------------------------------
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_aBillboard[ nLoop ].mtxWorld );

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_aBillboard[ nLoop ].scl.x, g_aBillboard[ nLoop ].scl.y, g_aBillboard[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aBillboard[ nLoop ].mtxWorld, &g_aBillboard[ nLoop ].mtxWorld, &mtxScl );

			// �t�s������߂�
			D3DXMatrixInverse( &g_aBillboard[ nLoop ].mtxWorld,	// ���[���h�}�g���b�N�X
							   NULL,
							   &Camera.mtxView );				// �r���[�}�g���b�N�X

			g_aBillboard[ nLoop ].mtxWorld._41 = 0.0f;
			g_aBillboard[ nLoop ].mtxWorld._42 = 0.0f;
			g_aBillboard[ nLoop ].mtxWorld._43 = 0.0f;

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTrans, g_aBillboard[ nLoop ].pos.x, g_aBillboard[ nLoop ].pos.y, g_aBillboard[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aBillboard[ nLoop ].mtxWorld, &g_aBillboard[ nLoop ].mtxWorld, &mtxTrans );

			// ���[���h�}�g���b�N�X�̐ݒ�
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aBillboard[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// �|���S���`�揈��
			//--------------------

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// ���_�t�H�[�}�b�g
			D3DDevice -> SetStreamSource( 0,
										  g_aBillboard[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// �e�N�X�`���̐ݒ�
			D3DDevice -> SetTexture( 0, g_aBillboard[ 0 ].pD3DTexture );

			// �A���t�@�e�X�g
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// �A���t�@�e�X�g��L���ɂ���
			D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// ��r����A���t�@�l�i 0�`255 �j
			D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// ��r����

			
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			//--------------------

			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// �A���t�@�e�X�g�𖳌��ɂ���
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

void UnInitBillboard( void )
{
	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		// �r���{�[�h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_aBillboard[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aBillboard[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �r���{�[�h�̃e�N�X�`���̏I������
		if( g_aBillboard[ nLoop ].pD3DTexture != NULL )
		{
			g_aBillboard[ nLoop ].pD3DTexture -> Release();
			g_aBillboard[ nLoop ].pD3DTexture = NULL;
		}
	}
}

//============================================================================
//
// �r���{�[�h�̏��擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_aBillboard
//
//============================================================================

BILLBOARD* GetBillboard( void )
{
	return g_aBillboard;
}

void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if( g_aBillboard[ nLoop ].bUse == false )
		{
			// �T�E���h�Đ�


			// �e��ݒ�
			g_aBillboard[ nLoop ].pos.x  = pos.x;
			g_aBillboard[ nLoop ].pos.y  = pos.y + 30.0f;
			g_aBillboard[ nLoop ].pos.z  = pos.z;
			g_aBillboard[ nLoop ].Move.y = rot.y;
			g_aBillboard[ nLoop ].nShadowNumber = SetShadow( g_aBillboard[ nLoop ].pos, g_aBillboard[ nLoop ].fShadowSize );
			g_aBillboard[ nLoop ].bUse   = true;
			break;
		}
	}
}

/* EOF */