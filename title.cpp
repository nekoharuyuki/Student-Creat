//===============================================================================================================
//
// title.cpp
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
#include "title.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

void MakeVertexTitle(void);

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureTitle    = NULL;		// �e�N�X�`���ւ̃|�C���^(Title�p)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTitle    = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^(Title�p)

static LPDIRECT3DTEXTURE9		g_pD3DTextureTitle_2  = NULL;		// �e�N�X�`���ւ̃|�C���^(Title�p)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTitle_2  = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^(Title�p)

static LPDIRECT3DTEXTURE9		g_pD3DTextureEnter    = NULL;		// �e�N�X�`���ւ̃|�C���^(Enter�p)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffEnter    = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^(Enter�p)

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@HRESULT hr
//
//============================================================================

HRESULT InitTitle(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexTitle();

	HRESULT hr;

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_TITLE_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureTitle);	// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_TITLE_2_FILE,	// �t�@�C���̖��O
									&g_pD3DTextureTitle_2);	// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_ENTER_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureEnter);	// �ǂݍ��ރ������[

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
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

void MakeVertexTitle(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffTitle,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffTitle_2,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

    D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffEnter,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�
	
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffTitle -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffTitle -> Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffTitle_2 -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffTitle_2 -> Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffEnter -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffEnter -> Unlock();
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

void UpdateTitle(void)
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

void DrawTitle(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	int nTime = GetTime();

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	if( nTime % 2 == 0 )
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTitle, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureTitle );

		// �|���S���̕`��
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffEnter, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureEnter );

		// �|���S���̕`��
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
	}
	else
	{
		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTitle_2, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureTitle_2 );

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

void UninitTitle(void)
{
	// �e�N�X�`���̊J��
	if( g_pD3DTextureTitle != NULL )
	{
		g_pD3DTextureTitle -> Release();
		g_pD3DTextureTitle = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffTitle != NULL )
	{
		g_pD3DVtxBuffTitle -> Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	// �e�N�X�`���̊J��
	if( g_pD3DTextureTitle_2 != NULL )
	{
		g_pD3DTextureTitle_2 -> Release();
		g_pD3DTextureTitle_2 = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffTitle_2 != NULL )
	{
		g_pD3DVtxBuffTitle_2 -> Release();
		g_pD3DVtxBuffTitle_2 = NULL;
	}

	// �e�N�X�`���̊J��
	if( g_pD3DTextureEnter != NULL )
	{
		g_pD3DTextureEnter -> Release();
		g_pD3DTextureEnter = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffEnter != NULL )
	{
		g_pD3DVtxBuffEnter -> Release();
		g_pD3DVtxBuffEnter = NULL;
	}
}

/* EOF */