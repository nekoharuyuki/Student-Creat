//===============================================================================================================
//
// mashwall.cpp
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
#include "meshwall.h"
#include "meshfield.h"
#include "dungeon.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexSky( void );		// ��̒��_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

MESH_WALL g_aMeshSky[ MAX_SKY ];
int g_nNumMeshSky = 0;

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	�ǂ̐ݒ�
//	 S_OK	: �ǂ̐ݒ�ɐ���
//	 E_FAIL	: �ǂ̐ݒ�Ɏ��s
//
//============================================================================

HRESULT InitSky( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_nNumMeshSky = 0;

	HRESULT hr;

	for( int nLoop = 0; nLoop < MAX_SKY; nLoop++ )
	{
		// �Ǘp�̍\���̕ϐ��̏�����
		//-----------------------------------

		// ���_��
		g_aMeshSky[ nLoop ].nNumVertex  = ( ( TEXTURE_NUM * WORLD + 1 ) * ( TEXTURE_NUM * WORLD + 1 ) );

		// �C���f�b�N�X��
		g_aMeshSky[ nLoop ].nNumIndex   = ( ( ( g_aMeshSky[ nLoop ].nNumVertex - 2 ) * 2 ) );

		// �|���S����
		g_aMeshSky[ nLoop ].nNumPolygon = TEXTURE_NUM;

		g_aMeshSky[ nLoop ].nNumBlockX  = TEXTURE_SIZE;
		g_aMeshSky[ nLoop ].nNumBlockY  = TEXTURE_SIZE;
		g_aMeshSky[ nLoop ].fSizeBlockX = ( -( (float)g_aMeshSky[ nLoop ].nNumPolygon / 2 ) * g_aMeshSky[ nLoop ].nNumBlockX );
		g_aMeshSky[ nLoop ].fSizeBlockY = ( -( (float)g_aMeshSky[ nLoop ].nNumPolygon / 2 ) * g_aMeshSky[ nLoop ].nNumBlockY );
		g_aMeshSky[ nLoop ].bUse        = false;

		// �ǂ̈ʒu�i�����A�傫���j������
		//-----------------------------------

		// �ǂ̈ʒu�E����
		// ��
		g_aMeshSky[ 0 ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aMeshSky[ 0 ].pos = D3DXVECTOR3( 0.0f, ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 150.f );

		// �E
		g_aMeshSky[ 1 ].rot = D3DXVECTOR3( 0.0f, ( D3DX_PI / 2 ), 0.0f );
		g_aMeshSky[ 1 ].pos = D3DXVECTOR3( ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 150.f, ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 300.f );
		
		// ��
		g_aMeshSky[ 2 ].rot = D3DXVECTOR3( 0.0f, -( D3DX_PI / 2 ), 0.0f );
		g_aMeshSky[ 2 ].pos = D3DXVECTOR3( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), 0.0f );

		// �ǂ̑傫��
		g_aMeshSky[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		//-----------------------------------

		// �e�N�X�`���̓ǂݍ���
		hr = D3DXCreateTextureFromFile( D3DDevice,		// �ǂ̃e�N�X�`���t�@�C��
										WALL_SKY,
										&g_aMeshSky[ nLoop ].pD3DTexture );

		if( FAILED( hr ) ) 
		{
			return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
		}

		// �ǂ̐�
		g_nNumMeshSky++;
	}

	// ���_���̐ݒ�
	hr = MakeVertexSky();

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

HRESULT MakeVertexSky( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		//------------------------------
		// ���_�o�b�t�@�̐���
		//------------------------------

		// ���_�o�b�t�@�̍쐬
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * g_aMeshSky[ nLoop ].nNumVertex,	// ���_�o�b�t�@�̃T�C�Y
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		if( FAILED( hr ) )
		{
			return E_FAIL;	// ���_�o�b�t�@�̍쐬�Ɏ��s
		}

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, ( void** )&pVtx, 0 );

		// 3D�̒��_���W�i X, Y, Z �j�̐ݒ�
		//-----------------------------------

		int nCountVtx = 0;
		float fTemp = g_aMeshSky[ nLoop ].fSizeBlockY;

		for( int nCountVtx_Y = 0; nCountVtx_Y <= TEXTURE_NUM * WORLD; nCountVtx_Y++ )
		{
			for( int nCountVtx_X = 0; nCountVtx_X <= TEXTURE_NUM * WORLD; nCountVtx_X++ )
			{
				pVtx[ nCountVtx ].vtx = D3DXVECTOR3( g_aMeshSky[ nLoop ].fSizeBlockX + ( nCountVtx_X * TEXTURE_SIZE ) , -( fTemp ), 0.f );
				nCountVtx++;
			}
			fTemp += TEXTURE_SIZE;
		}

		// �@���x�N�g���E���ˌ��̐ݒ�
		//-----------------------------------

		for( int nCount = 0; nCount < g_aMeshSky[ nLoop ].nNumVertex; nCount++ )
		{
			pVtx[ nCount ].nor      =  D3DXVECTOR3( 0, 0, -1.0f );
			pVtx[ nCount ].diffuse  =  D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		// �e�N�X�`�����W�̐ݒ�
		//-----------------------------------

		int nCountTex = 0;

		for( int nTex_Y = 0; nTex_Y <= TEXTURE_NUM * WORLD; nTex_Y++ )
		{
			for( int nTex_X = 0; nTex_X <= TEXTURE_NUM * WORLD; nTex_X++ )
			{
				pVtx[ nCountTex ].tex = D3DXVECTOR2( ( float ) nTex_X, ( float ) nTex_Y );
				nCountTex++;
			}
		}

		g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------

		//------------------------------
		// �C���f�b�N�X�o�b�t�@�̐���
		//------------------------------

		// �C���f�b�N�X���̍쐬
		hr = D3DDevice -> CreateIndexBuffer( sizeof( WORD ) * g_aMeshSky[ nLoop ].nNumIndex,	// �C���f�b�N�X �o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY,
											 D3DFMT_INDEX16,
											 D3DPOOL_MANAGED,
											 &g_aMeshSky[ nLoop ].pD3DIndxBuff,
											 NULL);

		if( FAILED( hr ) )
		{
			return E_FAIL;	// �C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s
		}

		// �C���f�b�N�X���̐ݒ�
		//-----------------------------------

		WORD *pIdx;
		g_aMeshSky[ nLoop ].pD3DIndxBuff ->Lock( 0, 0, ( void** )&pIdx, 0 );

		int nOddNumber   = 1;		// �����ԍ��̓��͗p�ϐ�
		int nEvenNumber  = 0;		// ��ԍ��̓��͗p�ϐ�

		// �k�ރ|���S���v�Z�p�ϐ�
		int nDegeneracy = ( ( TEXTURE_NUM * WORLD + 1 ) * 2 );
		int nDegeneracy_temp = nDegeneracy + 1;
		int nNext_Degeneracy = nDegeneracy_temp + 1;

		for( int nCountIdx = 0; nCountIdx < g_aMeshSky[ nLoop ].nNumIndex; nCountIdx++ )
		{
			// �k�ރ|���S���p�̏���
			if( nCountIdx == nDegeneracy )
			{
				pIdx[ nCountIdx ] = nEvenNumber - 1;
				nDegeneracy += nNext_Degeneracy;
			}
			else if( nCountIdx == nDegeneracy_temp )
			{
				pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM * WORLD;
				nDegeneracy_temp += nNext_Degeneracy;
			}
			else
			{
				if( nCountIdx % 2 == 0 )
				{
					// �����ԍ��ɓ���l
					pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM * WORLD;
					nOddNumber++;
				}
				if( nCountIdx % 2 == 1 )
				{
					// ��ԍ��ɓ���l
					pIdx[ nCountIdx ] = nEvenNumber;
					nEvenNumber++;
				}
			}	
		}

		g_aMeshSky[ nLoop ].pD3DIndxBuff -> Unlock();

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

void UpdataSky( void )
{
	;
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

void DrawSky( void )
{
	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		// DirectX�f�o�C�X�̒l���擾
		LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

		D3DXMATRIX mtxScl, mtxRot, mtxTrans;

		//------------------------------
		// ���[���h�}�g���b�N�X�̐ݒ�
		//------------------------------

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &g_aMeshSky[ nLoop ].mtxWorld );

		// �X�P�[���𔽉f
		D3DXMatrixScaling( &mtxScl, g_aMeshSky[ nLoop ].scl.x, g_aMeshSky[ nLoop ].scl.y, g_aMeshSky[ nLoop ].scl.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxScl );

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aMeshSky[ nLoop ].rot.y, g_aMeshSky[ nLoop ].rot.x, g_aMeshSky[ nLoop ].rot.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxRot );

		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTrans, g_aMeshSky[ nLoop ].pos.x, g_aMeshSky[ nLoop ].pos.y, g_aMeshSky[ nLoop ].pos.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxTrans );

		// ���[���h�}�g���b�N�X�̐ݒ�
		D3DDevice -> SetTransform( D3DTS_WORLD, &g_aMeshSky[ nLoop ].mtxWorld );

		//------------------------------

		//--------------------
		// �|���S���`�揈��
		//--------------------

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetFVF( FVF_VERTEX_3D );

		D3DDevice -> SetIndices( g_aMeshSky[ nLoop ].pD3DIndxBuff );

		// ���_�t�H�[�}�b�g
		D3DDevice -> SetStreamSource( 0,
									  g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer,
									  0,
									  sizeof( VERTEX_3D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_aMeshSky[ nLoop ].pD3DTexture );

		// �|���S���̕`��
		D3DDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	// �����_�����O����v���~�e�B�u�̎��
										   0,
										   0,
										   g_aMeshSky[ nLoop ].nNumVertex,			// �g�p����钸�_�̐�
										   0,
										   g_aMeshSky[ nLoop ].nNumIndex - 2 );	// �����_�����O����v���~�e�B�u�̐�

		//--------------------
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

void UnInitSky( void )
{
	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		// �t�B�[���h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �t�B�[���h�̃C���f�b�N�X�o�b�t�@�̏I������
		if( g_aMeshSky[ nLoop ].pD3DIndxBuff != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DIndxBuff -> Release();
			g_aMeshSky[ nLoop ].pD3DIndxBuff = NULL;
		}
		// �t�B�[���h�̃e�N�X�`���̏I������
		if( g_aMeshSky[ nLoop ].pD3DTexture != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DTexture -> Release();
			g_aMeshSky[ nLoop ].pD3DTexture = NULL;
		}
	}
}

/* EOF */