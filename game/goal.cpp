//===============================================================================================================
//
// goal.cpp
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
#include "camera.h"
#include "meshfield.h"
#include "shadow.h"
#include "goal.h"
#include "dungeon.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexGoal( void );		// ���_���̍쐬

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static BILLBOARD g_Goal;

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

HRESULT InitGoal( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// �e�̈ʒu�i�����A�傫���j������
	g_Goal.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// ����
	g_Goal.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// �傫��

	g_Goal.fShadowSize = SHADOW_SIZE_GOAL;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// �_���W�����̏��擾
		int Dungeon = GetDungeon( ( int )( nLoop / FIELD_X ), ( int )( nLoop % FIELD_Z ) );

		// �t�B�[���h�̃|�W�V����
		if( Dungeon == 2 )
		{
			g_Goal.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_Goal.pos = D3DXVECTOR3( ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop / FIELD_X ) ),
									  GOAL_SIZE / 2,
									  ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop % FIELD_Z ) ) );
			g_Goal.bUse = true;
		}
	}

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice, GOAL_FILE, &g_Goal.pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}

	// ���_���̐ݒ�
	hr = MakeVertexGoal();

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

HRESULT MakeVertexGoal( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// ���_�o�b�t�@�̐���
	//------------------------------

	// ���_�o�b�t�@�̍쐬
	hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
										  D3DUSAGE_WRITEONLY,
										  FVF_VERTEX_3D,
										  D3DPOOL_MANAGED,
										  &g_Goal.pD3DVtxBtxBuffer,
										  NULL );

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	g_Goal.pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx[0].vtx = D3DXVECTOR3( -GOAL_SIZE,  GOAL_SIZE, 0.0f );
	pVtx[1].vtx = D3DXVECTOR3(  GOAL_SIZE,  GOAL_SIZE, 0.0f );
	pVtx[2].vtx = D3DXVECTOR3( -GOAL_SIZE, -GOAL_SIZE, 0.0f );
	pVtx[3].vtx = D3DXVECTOR3(  GOAL_SIZE, -GOAL_SIZE, 0.0f );

	for( int i = 0; i < 4; i++ )
	{
		pVtx[ i ].nor = D3DXVECTOR3( 0, 1, 0 );
		pVtx[ i ].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	}

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	g_Goal.pD3DVtxBtxBuffer -> Unlock();

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

void UpdataGoal( void )
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

void DrawGoal( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �J�����̏����擾
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	if(g_Goal.bUse == true)
	{
		//------------------------------
		// ���[���h�}�g���b�N�X�̐ݒ�
		//------------------------------

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &g_Goal.mtxWorld );

		// �X�P�[���𔽉f
		D3DXMatrixScaling( &mtxScl, g_Goal.scl.x, g_Goal.scl.y, g_Goal.scl.z );
		D3DXMatrixMultiply( &g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxScl );

		// �t�s������߂�
		D3DXMatrixInverse( &g_Goal.mtxWorld,	// ���[���h�}�g���b�N�X
						   NULL,
						   &Camera.mtxView );	// �r���[�}�g���b�N�X

		g_Goal.mtxWorld._41 = 0.0f;
		g_Goal.mtxWorld._42 = 0.0f;
		g_Goal.mtxWorld._43 = 0.0f;

		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTrans, g_Goal.pos.x, g_Goal.pos.y, g_Goal.pos.z );
		D3DXMatrixMultiply( &g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxTrans );

		// ���[���h�}�g���b�N�X�̐ݒ�
		D3DDevice -> SetTransform( D3DTS_WORLD, &g_Goal.mtxWorld );

		//------------------------------

		//--------------------
		// �|���S���`�揈��
		//--------------------

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetFVF( FVF_VERTEX_3D );

		// ���_�t�H�[�}�b�g
		D3DDevice -> SetStreamSource( 0,
									  g_Goal.pD3DVtxBtxBuffer,
									  0,
									  sizeof( VERTEX_3D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_Goal.pD3DTexture );

		// �A���t�@�e�X�g
		D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// �A���t�@�e�X�g��L���ɂ���
		D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// ��r����A���t�@�l�i 0�`255 �j
		D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// ��r����

		
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );

		//--------------------

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

		// �f�o�b�O���\��
		PrintDebugProc("GOAL POS  : ( %.2f : %.2f : %.2f )\n", g_Goal.pos.x, g_Goal.pos.y, g_Goal.pos.z);

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

		D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// �A���t�@�e�X�g�𖳌��ɂ���
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

void UnInitGoal( void )
{
	// �r���{�[�h�̃o�[�e�b�N�X�o�b�t�@�̏I������
	if( g_Goal.pD3DVtxBtxBuffer != NULL )
	{
		g_Goal.pD3DVtxBtxBuffer -> Release();
		g_Goal.pD3DVtxBtxBuffer = NULL;
	}
	// �r���{�[�h�̃e�N�X�`���̏I������
	if( g_Goal.pD3DTexture != NULL )
	{
		g_Goal.pD3DTexture -> Release();
		g_Goal.pD3DTexture = NULL;
	}
}

//============================================================================
//
// �r���{�[�h�̏��擾�@(�S�[��) ���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_Goal
//
//============================================================================

BILLBOARD GetGoal(void)
{
	return g_Goal;
}

/* EOF */