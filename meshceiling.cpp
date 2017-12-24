//===============================================================================================================
//
// mashceiling.cpp
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
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "dungeon.h"

//============================================================================
//
// �}�N����`
//
//============================================================================

// ���_��
#define CEILING_VERTEX            ( ( TEXTURE_NUM + 1 ) * ( TEXTURE_NUM + 1 ) )

// �C���f�b�N�X��
#define CEILING_INDEX             ( ( ( CEILING_VERTEX - 2 ) * 2 ) )

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexCeiling( void );		// �t�B�[���h�̒��_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static MESH_FIELD g_Ceiling[ MAX_FIELD ];

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

HRESULT InitCeiling( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// �n�ʂ̈ʒu�i�����A�傫���j������
		g_Ceiling[ nLoop ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// �n�ʂ̈ʒu
		g_Ceiling[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// �n�ʂ̌���
		g_Ceiling[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// �n�ʂ̑傫��

		g_Ceiling[ nLoop ].bUse     = false;
	}

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice, WALL_FILE, &g_Ceiling[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

	// ���_���̐ݒ�
	hr = MakeVertexCeiling();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// ���_���̐ݒ�Ɏ��s
	}

	// �_���W�����쐬

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// �_���W�����̏��擾
		int Dungeon = GetDungeon( ( int )( nLoop / FIELD_X ), ( int )( nLoop % FIELD_Z ) );

		// �t�B�[���h�̃|�W�V����
		if( Dungeon == 1 )
		{
			g_Ceiling[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_Ceiling[ nLoop ].pos = D3DXVECTOR3( ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop / FIELD_X ) ),
												  ( float )( ( TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) + 50,
												  ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop % FIELD_Z ) ) );
			g_Ceiling[ nLoop ].bUse = true;
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

HRESULT MakeVertexCeiling( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// ���_�o�b�t�@�̐���
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// ���_�o�b�t�@�̍쐬
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * CEILING_VERTEX,	// ���_�o�b�t�@�̃T�C�Y
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_Ceiling[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		if( FAILED( hr ) )
		{
			return E_FAIL;	// ���_�o�b�t�@�̍쐬�Ɏ��s
		}

		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, ( void** )&pVtx, 0 );

		// 3D�̒��_���W�i X, Y, Z �j�̐ݒ�
		//-----------------------------------

		int nCountVtx = 0;

		float fPoint = ( -( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE );
		float fTemp = fPoint;

		for( int nCountVtx_Z = 0; nCountVtx_Z <= TEXTURE_NUM; nCountVtx_Z++ )
		{
			for( int nCountVtx_X = 0; nCountVtx_X <= TEXTURE_NUM; nCountVtx_X++ )
			{
				pVtx[ nCountVtx ].vtx = D3DXVECTOR3( fPoint + ( nCountVtx_X * TEXTURE_SIZE ) , 0.f, -( fTemp ) );
				nCountVtx++;
			}
			fTemp += TEXTURE_SIZE;
		}

		// �@���x�N�g���E���ˌ��̐ݒ�
		//-----------------------------------

		for( int i = 0; i < CEILING_VERTEX; i++ )
		{
			pVtx[ i ].nor      =  D3DXVECTOR3( 0, 1, 0 );
			pVtx[ i ].diffuse  =  D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		// �e�N�X�`�����W�̐ݒ�
		//-----------------------------------

		int nCount = 0;

		for( int nTex_Y = 0; nTex_Y <= TEXTURE_NUM; nTex_Y++ )
		{
			for( int nTex_X = 0; nTex_X <= TEXTURE_NUM; nTex_X++ )
			{
				pVtx[nCount].tex = D3DXVECTOR2( ( float ) nTex_X, ( float ) nTex_Y );
				nCount++;
			}
		}

		g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------

		//------------------------------
		// �C���f�b�N�X�o�b�t�@�̐���
		//------------------------------

		// �C���f�b�N�X���̍쐬
		hr = D3DDevice -> CreateIndexBuffer( sizeof( WORD ) * CEILING_INDEX,	// �C���f�b�N�X �o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY,
											 D3DFMT_INDEX16,
											 D3DPOOL_MANAGED,
											 &g_Ceiling[ nLoop ].pD3DIndxBuff,
											 NULL);

		if( FAILED( hr ) )
		{
			return E_FAIL;	// �C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s
		}

		// �C���f�b�N�X���̐ݒ�
		//-----------------------------------

		WORD *pIdx;
		g_Ceiling[ nLoop ].pD3DIndxBuff -> Lock( 0, 0, ( void** )&pIdx, 0 );

		int nOddNumber   = 1;		// �����ԍ��̓��͗p�ϐ�
		int nEvenNumber  = 0;		// ��ԍ��̓��͗p�ϐ�

		// �k�ރ|���S���v�Z�p�ϐ�
		int nDegeneracy      = ( ( TEXTURE_NUM + 1 ) * 2 );
		int nDegeneracy_temp = nDegeneracy + 1;
		int nNext_Degeneracy = nDegeneracy_temp + 1;

		for( int nCountIdx = 0; nCountIdx < CEILING_INDEX; nCountIdx++ )
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

		g_Ceiling[ nLoop ].pD3DIndxBuff -> Unlock();
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

void UpdataCeiling( void )
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

void DrawCeiling( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		
		if( g_Ceiling[ nLoop ].bUse == true )
		{
			//------------------------------
			// ���[���h�}�g���b�N�X�̐ݒ�
			//------------------------------
		
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_Ceiling[ nLoop ].mtxWorld );

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_Ceiling[ nLoop ].scl.x, g_Ceiling[ nLoop ].scl.y, g_Ceiling[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxScl );

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_Ceiling[ nLoop ].rot.y, g_Ceiling[ nLoop ].rot.x, g_Ceiling[ nLoop ].rot.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxRot );

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTrans, g_Ceiling[ nLoop ].pos.x, g_Ceiling[ nLoop ].pos.y, g_Ceiling[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxTrans );

			// ���[���h�}�g���b�N�X�̐ݒ�
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_Ceiling[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// �|���S���`�揈��
			//--------------------

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			D3DDevice -> SetIndices( g_Ceiling[ nLoop ].pD3DIndxBuff );

			// ���_�t�H�[�}�b�g
			D3DDevice -> SetStreamSource( 0,
										  g_Ceiling[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// �e�N�X�`���̐ݒ�
			D3DDevice -> SetTexture( 0, g_Ceiling[ 0 ].pD3DTexture );

			D3DDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	// �����_�����O����v���~�e�B�u�̎��
											   0,
											   0,
											   CEILING_VERTEX,		// �g�p����钸�_�̐�
											   0,
											   CEILING_INDEX - 2 );	// �����_�����O����v���~�e�B�u�̐�

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

void UnInitCeiling( void )
{
	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// �t�B�[���h�̃o�[�e�b�N�X�o�b�t�@�̏I������
		if( g_Ceiling[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_Ceiling[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// �t�B�[���h�̃C���f�b�N�X�o�b�t�@�̏I������
		if( g_Ceiling[ nLoop ].pD3DIndxBuff != NULL )
		{
			g_Ceiling[ nLoop ].pD3DIndxBuff -> Release();
			g_Ceiling[ nLoop ].pD3DIndxBuff = NULL;
		}
		// �t�B�[���h�̃e�N�X�`���̏I������
		if( g_Ceiling[ nLoop ].pD3DTexture != NULL )
		{
			g_Ceiling[ nLoop ].pD3DTexture -> Release();
			g_Ceiling[ nLoop ].pD3DTexture = NULL;
		}
	}
}

MESH_FIELD GetCeiling( int nIdx )
{
	return g_Ceiling[ nIdx ];
}