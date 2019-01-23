//===============================================================================================================
//
// readygo.cpp
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
#include "game.h"
#include "readygo.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

void MakeVertexReadGo(void);

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureRead = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffRead = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static LPDIRECT3DTEXTURE9		g_pD3DTextureGo   = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffGo   = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@HRESULT hr
//
//============================================================================

HRESULT InitReadGo(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_READY_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureRead);	// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_GO_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureGo);		// �ǂݍ��ރ������[

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

	// ���_���̍쐬
	MakeVertexReadGo();

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

void MakeVertexReadGo(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffRead,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffGo,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�
	
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffRead -> Lock( 0, 0, (void**)&pVtx, 0 );

	// ���W�w��
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffRead -> Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffGo -> Lock( 0, 0, (void**)&pVtx, 0 );

	// ���W�w��
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffGo -> Unlock();
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

void UpdateReadGo(void)
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

void DrawReadGo(void)
{
	int nTime = GetGameTime();

	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	if( nTime == TIME_LIMIT )
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffRead, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureRead );

		// �|���S���̕`��
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
	}

	if( nTime == TIME_LIMIT - 1 )
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffGo, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureGo );

		// �|���S���̕`��
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
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

void UninitReadGo(void)
{
	// �e�N�X�`���̊J��
	if( g_pD3DVtxBuffRead != NULL )
	{
		g_pD3DVtxBuffRead -> Release();
		g_pD3DVtxBuffRead = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DTextureRead != NULL )
	{
		g_pD3DTextureRead -> Release();
		g_pD3DTextureRead = NULL;
	}

	// �e�N�X�`���̊J��
	if( g_pD3DTextureGo != NULL )
	{
		g_pD3DTextureGo -> Release();
		g_pD3DTextureGo = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffGo != NULL )
	{
		g_pD3DVtxBuffGo -> Release();
		g_pD3DVtxBuffGo = NULL;
	}
}

/* EOF */