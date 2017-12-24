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

HRESULT MakeVertexWallLeft( void );		// �ǂ̒��_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static MESH_WALL g_aMeshWall_Left[ MAX_WALL * MAX_FIELD ];
static int g_nNumMeshWall_Left = 0;

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

HRESULT InitWallLeft( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_nNumMeshWall_Left = 0;

	HRESULT hr;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// �Ǘp�̍\���̕ϐ��̏�����
		//-----------------------------------

		// ���_��
		g_aMeshWall_Left[ nLoop ].nNumVertex  = ( ( TEXTURE_NUM + 1 ) * ( TEXTURE_NUM + 1 ) );

		// �C���f�b�N�X��
		g_aMeshWall_Left[ nLoop ].nNumIndex   = ( ( ( g_aMeshWall_Left[ nLoop ].nNumVertex - 2 ) * 2 ) );

		// �|���S����
		g_aMeshWall_Left[ nLoop ].nNumPolygon = TEXTURE_NUM;

		g_aMeshWall_Left[ nLoop ].nNumBlockX  = TEXTURE_SIZE;
		g_aMeshWall_Left[ nLoop ].nNumBlockY  = TEXTURE_SIZE;
		g_aMeshWall_Left[ nLoop ].fSizeBlockX = ( -( (float)g_aMeshWall_Left[ nLoop ].nNumPolygon / 2 ) * g_aMeshWall_Left[ nLoop ].nNumBlockX );
		g_aMeshWall_Left[ nLoop ].fSizeBlockY = ( -( (float)g_aMeshWall_Left[ nLoop ].nNumPolygon / 2 ) * g_aMeshWall_Left[ nLoop ].nNumBlockY );
		g_aMeshWall_Left[ nLoop ].bUse        = false;

		// �ǂ̈ʒu�i�����A�傫���j������
		//-----------------------------------

		// �ǂ̈ʒu�E����
		g_aMeshWall_Left[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aMeshWall_Left[ nLoop ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		// �ǂ̑傫��
		g_aMeshWall_Left[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		//-----------------------------------

		// �ǂ̐�
		g_nNumMeshWall_Left++;
	}

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice,		// �ǂ̃e�N�X�`���t�@�C��
									WALL_FILE,
									&g_aMeshWall_Left[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

	// ���_���̐ݒ�
	hr = MakeVertexWallLeft();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// ���_���̐ݒ�Ɏ��s
	}

	// �_���W�����쐬

	for( int nLoop = 0; nLoop < g_nNumMeshWall_Left; nLoop++ )
	{
		// �_���W�����̏��擾
		int Dungeon = GetDungeon( ( int )( nLoop / FIELD_X ), ( int )( nLoop % FIELD_Z ) );

		// �t�B�[���h�̃|�W�V����
		if( Dungeon == 1 )
		{
			g_aMeshWall_Left[ nLoop ].rot = D3DXVECTOR3( 0.0f, -( D3DX_PI / 2 ), 0.0f );
			g_aMeshWall_Left[ nLoop ].pos = D3DXVECTOR3( ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop / FIELD_X ) ),
														 0.0f,
														 ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop % FIELD_Z ) ) );
			g_aMeshWall_Left[ nLoop ].bUse = true;
		}
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

HRESULT MakeVertexWallLeft( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < g_nNumMeshWall_Left; nLoop++ )
	{
		//------------------------------
		// ���_�o�b�t�@�̐���
		//------------------------------

		// ���_�o�b�t�@�̍쐬
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * g_aMeshWall_Left[ nLoop ].nNumVertex,	// ���_�o�b�t�@�̃T�C�Y
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		if( FAILED( hr ) )
		{
			return E_FAIL;	// ���_�o�b�t�@�̍쐬�Ɏ��s
		}

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, ( void** )&pVtx, 0 );

		// 3D�̒��_���W�i X, Y, Z �j�̐ݒ�
		//-----------------------------------

		int nCountVtx = 0;
		float fTemp = g_aMeshWall_Left[ nLoop ].fSizeBlockY;

		for( int nCountVtx_Y = 0; nCountVtx_Y <= TEXTURE_NUM; nCountVtx_Y++ )
		{
			for( int nCountVtx_X = 0; nCountVtx_X <= TEXTURE_NUM; nCountVtx_X++ )
			{
				pVtx[ nCountVtx ].vtx = D3DXVECTOR3( g_aMeshWall_Left[ nLoop ].fSizeBlockX + ( nCountVtx_X * TEXTURE_SIZE ) , -( fTemp ), g_aMeshWall_Left[ nLoop ].fSizeBlockY );
				nCountVtx++;
			}
			fTemp += TEXTURE_SIZE;
		}

		// �@���x�N�g���E���ˌ��̐ݒ�
		//-----------------------------------

		for( int nCount = 0; nCount < g_aMeshWall_Left[ nLoop ].nNumVertex; nCount++ )
		{
			pVtx[ nCount ].nor      =  D3DXVECTOR3( 0, 0, -1.0f );
			pVtx[ nCount ].diffuse  =  D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		// �e�N�X�`�����W�̐ݒ�
		//-----------------------------------

		int nCountTex = 0;

		for( int nTex_Y = 0; nTex_Y <= TEXTURE_NUM; nTex_Y++ )
		{
			for( int nTex_X = 0; nTex_X <= TEXTURE_NUM; nTex_X++ )
			{
				pVtx[ nCountTex ].tex = D3DXVECTOR2( ( float ) nTex_X, ( float ) nTex_Y );
				nCountTex++;
			}
		}

		g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------

		//------------------------------
		// �C���f�b�N�X�o�b�t�@�̐���
		//------------------------------

		// �C���f�b�N�X���̍쐬
		hr = D3DDevice -> CreateIndexBuffer( sizeof( WORD ) * g_aMeshWall_Left[ nLoop ].nNumIndex,	// �C���f�b�N�X �o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY,
											 D3DFMT_INDEX16,
											 D3DPOOL_MANAGED,
											 &g_aMeshWall_Left[ nLoop ].pD3DIndxBuff,
											 NULL);

		if( FAILED( hr ) )
		{
			return E_FAIL;	// �C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s
		}

		// �C���f�b�N�X���̐ݒ�
		//-----------------------------------

		WORD *pIdx;
		g_aMeshWall_Left[ nLoop ].pD3DIndxBuff ->Lock( 0, 0, ( void** )&pIdx, 0 );

		int nOddNumber   = 1;		// �����ԍ��̓��͗p�ϐ�
		int nEvenNumber  = 0;		// ��ԍ��̓��͗p�ϐ�

		// �k�ރ|���S���v�Z�p�ϐ�
		int nDegeneracy = ( ( TEXTURE_NUM + 1 ) * 2 );
		int nDegeneracy_temp = nDegeneracy + 1;
		int nNext_Degeneracy = nDegeneracy_temp + 1;

		for( int nCountIdx = 0; nCountIdx < g_aMeshWall_Left[ nLoop ].nNumIndex; nCountIdx++ )
		{
			// �k�ރ|���S���p�̏���
			if( nCountIdx == nDegeneracy )
			{
				pIdx[ nCountIdx ] = nEvenNumber - 1;
				nDegeneracy += nNext_Degeneracy;
			}
			else if( nCountIdx == nDegeneracy_temp )
			{
				pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM;
				nDegeneracy_temp += nNext_Degeneracy;
			}
			else
			{
				if( nCountIdx % 2 == 0 )
				{
					// �����ԍ��ɓ���l
					pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM;
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

		g_aMeshWall_Left[ nLoop ].pD3DIndxBuff -> Unlock();

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

void UpdataWallLeft( void )
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

void DrawWallLeft( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < g_nNumMeshWall_Left; nLoop++ )
	{
		if( g_aMeshWall_Left[ nLoop ].bUse == true )
		{
			//------------------------------
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_aMeshWall_Left[ nLoop ].mtxWorld );

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_aMeshWall_Left[ nLoop ].scl.x, g_aMeshWall_Left[ nLoop ].scl.y, g_aMeshWall_Left[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aMeshWall_Left[ nLoop ].mtxWorld, &g_aMeshWall_Left[ nLoop ].mtxWorld, &mtxScl );

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aMeshWall_Left[ nLoop ].rot.y, g_aMeshWall_Left[ nLoop ].rot.x, g_aMeshWall_Left[ nLoop ].rot.z );
			D3DXMatrixMultiply( &g_aMeshWall_Left[ nLoop ].mtxWorld, &g_aMeshWall_Left[ nLoop ].mtxWorld, &mtxRot );

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTrans, g_aMeshWall_Left[ nLoop ].pos.x, g_aMeshWall_Left[ nLoop ].pos.y, g_aMeshWall_Left[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aMeshWall_Left[ nLoop ].mtxWorld, &g_aMeshWall_Left[ nLoop ].mtxWorld, &mtxTrans );

			// ���[���h�}�g���b�N�X�̐ݒ�
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aMeshWall_Left[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// �|���S���`�揈��
			//--------------------

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			D3DDevice -> SetIndices( g_aMeshWall_Left[ nLoop ].pD3DIndxBuff );

			// ���_�t�H�[�}�b�g
			D3DDevice -> SetStreamSource( 0,
										  g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// �e�N�X�`���̐ݒ�
			D3DDevice -> SetTexture( 0, g_aMeshWall_Left[ 0 ].pD3DTexture );

			// �|���S���̕`��
			D3DDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,						// �����_�����O����v���~�e�B�u�̎��
											   0,
											   0,
											   g_aMeshWall_Left[ nLoop ].nNumVertex,			// �g�p����钸�_�̐�
											   0,
											   g_aMeshWall_Left[ nLoop ].nNumIndex - 2 );	// �����_�����O����v���~�e�B�u�̐�
		}

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

void UnInitWallLeft( void )
{
	for( int nLoop = 0; nLoop < g_nNumMeshWall_Left; nLoop++ )
	{
		// �t�B�[���h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aMeshWall_Left[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �t�B�[���h�̃C���f�b�N�X�o�b�t�@�̏I������
		if( g_aMeshWall_Left[ nLoop ].pD3DIndxBuff != NULL )
		{
			g_aMeshWall_Left[ nLoop ].pD3DIndxBuff -> Release();
			g_aMeshWall_Left[ nLoop ].pD3DIndxBuff = NULL;
		}
		// �t�B�[���h�̃e�N�X�`���̏I������
		if( g_aMeshWall_Left[ nLoop ].pD3DTexture != NULL )
		{
			g_aMeshWall_Left[ nLoop ].pD3DTexture -> Release();
			g_aMeshWall_Left[ nLoop ].pD3DTexture = NULL;
		}
	}
}

/* EOF */