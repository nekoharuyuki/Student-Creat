//===============================================================================================================
//
// number.cpp
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
#include "number.h"
#include "game.h"

//============================================================================
//
// �}�N����`
//
//============================================================================

// �����P���̃e�N�X�`���T�C�Y
#define	TEXTURE_SIZE_X		( 448.0f )
#define	TEXTURE_SIZE_Y		( 289.0f )

// ���_��
#define NUM_NUMBER_POLYGON  ( 2 )
#define NUM_NUMBER_VERTEX   ( 4 )

// �����̎��
#define TEXTURE_DIV_X       ( 10 )
#define	NUM_NUMBERTYPE		( 4 )

//// �����̃T�C�Y
#define NUMBER_SIZE_X       ( TEXTURE_SIZE_X / TEXTURE_DIV_X  )
#define NUMBER_SIZE_Y       ( TEXTURE_SIZE_Y / NUM_NUMBERTYPE )

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexNumber(void);
void SetVertexNumber(int nIdxNumber);
void SetTextureNumber(int nIdxNumber);
void SetColorNumber(int nIdxNumber, D3DXCOLOR col);

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// ����
static LPDIRECT3DTEXTURE9		g_pD3DTextureNumber = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffNumber = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

// �t���[��
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrame = NULL;			// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffFrame = NULL;			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static NUMBER					g_aNumber[ MAX_NUM_NUMBER ];		// �����̃��[�N

// �X�R�A�p�ϐ�
static int g_score = 0;

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@HRESULT hr
//
//============================================================================

HRESULT InitNumber(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �����̏�����
	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		g_aNumber[ nCntNumber ].pos    = D3DXVECTOR3( NUMBER_POS_X, NUMBER_POS_Y, 0.0f );
		g_aNumber[ nCntNumber ].rot    = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aNumber[ nCntNumber ].nValue = 0;
		g_aNumber[ nCntNumber ].nType  = 0;
		g_aNumber[ nCntNumber ].bUse   = false;
	}

	// �X�R�A�p�ϐ��̏�����
	g_score = 0;

	// ���_���̍쐬
	MakeVertexNumber();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
							   TEXTURE_NUMBER,			// �t�@�C���̖��O
							   &g_pD3DTextureNumber );	// �ǂݍ��ރ������[

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

void UpdateNumber(void)
{
    int nDivision;
    int nRemainder;

    nRemainder = g_score % 10;
    nDivision  = g_score / 10;

    int nCntNumber = 0;

	GAMEMODE Mode = GetMode();

	float x;
	float y;

	if( Mode == SCENE_GAME )
	{
		x = NUMBER_POS_X;
		y = NUMBER_POS_Y;
	}
	else
	{
		if( Mode == SCENE_RECORD )
		{
			x = SCREEN_WIDTH  / 2;
			y = SCREEN_HEIGHT / 2;
		}
		else
		{
			x = 0;
			y = 0;
		}
	}

    do
    {
		// �����̍쐬
		g_aNumber[ nCntNumber ].nValue = nRemainder;				// �l
		g_aNumber[ nCntNumber ].bUse   = true;						// �g�p���t���O
		g_aNumber[ nCntNumber ].pos    = D3DXVECTOR3( x, y, 0.0f );	// �|�W�V����

		// ���_�̐ݒ�
		SetVertexNumber( nCntNumber );

		// �e�N�X�`���̐ݒ�
		SetTextureNumber( nCntNumber );

		nCntNumber++;

		x -= NUMBER_SIZE_X + 20;

		nRemainder = nDivision % 10;
		nDivision  = nDivision / 10;

    }while( nDivision > 0 || nRemainder > 0 );
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

void DrawNumber(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffNumber, 0, sizeof( VERTEX_2D ) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	D3DDevice -> SetTexture( 0, g_pD3DTextureNumber );

	// �|���S���̕`��
	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		if( g_aNumber[ nCntNumber ].bUse )
		{
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP, ( nCntNumber * 4 ), NUM_NUMBER_POLYGON );
		}
	}

	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		g_aNumber[nCntNumber].pos    = D3DXVECTOR3( NUMBER_POS_X, NUMBER_POS_Y, 0.0f );
		g_aNumber[nCntNumber].rot    = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aNumber[nCntNumber].nValue = 0;
		g_aNumber[nCntNumber].nType  = 0;
		g_aNumber[nCntNumber].bUse   = false;
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

void UninitNumber(void)
{
	// �e�N�X�`���̊J��
	if( g_pD3DTextureNumber != NULL )
	{
		g_pD3DTextureNumber -> Release();
		g_pD3DTextureNumber = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffNumber != NULL )
	{
		g_pD3DVtxBuffNumber -> Release();
		g_pD3DVtxBuffNumber = NULL;
	}
	// �e�N�X�`���̊J��
	if( g_pD3DTextureFrame != NULL )
	{
		g_pD3DTextureFrame -> Release();
		g_pD3DTextureFrame = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffFrame != NULL )
	{
		g_pD3DVtxBuffFrame -> Release();
		g_pD3DVtxBuffFrame = NULL;
	}
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

HRESULT MakeVertexNumber(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( D3DDevice->CreateVertexBuffer( sizeof(VERTEX_2D) * NUM_NUMBER_VERTEX * MAX_NUM_NUMBER,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
											   D3DUSAGE_WRITEONLY,										// ���_�o�b�t�@�̎g�p�@�@
											   FVF_VERTEX_3D,											// �g�p���钸�_�t�H�[�}�b�g
											   D3DPOOL_MANAGED,											// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
											   &g_pD3DVtxBuffNumber,									// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
											   NULL)))													// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffNumber -> Lock( 0, 0, (void**)&pVtx, 0 );

	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++, pVtx += 4 )
	{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x, g_aNumber[ nCntNumber ].pos.y, 0.0f );
		pVtx[1].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[nCntNumber].pos.y, 0.0f );
		pVtx[2].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x, g_aNumber[nCntNumber].pos.y + NUMBER_SIZE_Y, 0.0f );
		pVtx[3].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[nCntNumber].pos.y + NUMBER_SIZE_Y, 0.0f );

		// rhw�̐ݒ�
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// ���ˌ��̐ݒ�
		pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2( 0.0f,           0.0f );
		pVtx[1].tex = D3DXVECTOR2( TEXTURE_SIZE_X, 0.0f );
		pVtx[2].tex = D3DXVECTOR2( 0.0f,           TEXTURE_SIZE_Y );
		pVtx[3].tex = D3DXVECTOR2( TEXTURE_SIZE_X, TEXTURE_SIZE_Y );
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffNumber -> Unlock();

	return S_OK;
}

//============================================================================
//
// ���_�̐ݒ� �֐�
// �����F
//�@int nIdxNumber
// �߂�l�F
//	�Ȃ�
//�@
//============================================================================

void SetVertexNumber(int nIdxNumber)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffNumber -> Lock( 0, 0, ( void** )&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x, g_aNumber[ nIdxNumber ].pos.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[ nIdxNumber ].pos.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x, g_aNumber[ nIdxNumber ].pos.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[ nIdxNumber ].pos.y + NUMBER_SIZE_Y, 0.0f);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// �e�N�X�`���̐ݒ� �֐�
// �����F
//�@int nIdxNumber
// �߂�l�F
//	�Ȃ�
//�@
//============================================================================

void SetTextureNumber(int nIdxNumber)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffNumber -> Lock( 0, 0, ( void** )&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// ���_���W�̐ݒ�
    float x1, x2;
    x1 = 1.0f * g_aNumber[ nIdxNumber ].nValue / TEXTURE_DIV_X;
    x2 = 1.0f * ( g_aNumber[ nIdxNumber ].nValue + 1 ) / TEXTURE_DIV_X;

	pVtx[ 0 ].tex = D3DXVECTOR2( x1, 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( x2, 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( x1, 1.0f / NUM_NUMBERTYPE ); 
	pVtx[ 3 ].tex = D3DXVECTOR2( x2, 1.0f / NUM_NUMBERTYPE );

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// �J���[�̐ݒ� �֐�
// �����F
//�@int nIdxNumber
//	D3DXCOLOR col
// �߂�l�F
//	�Ȃ�
//�@
//============================================================================

void SetColorNumber(int nIdxNumber, D3DXCOLOR col)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffNumber -> Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// ���_���W�̐ݒ�
	pVtx[0].diffuse = 
	pVtx[1].diffuse = 
	pVtx[2].diffuse = 
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// �X�R�A�̐ݒ� �֐�
// �����F
//�@int n
// �߂�l�F
//	�Ȃ�
//�@
//============================================================================

void SetScore(int n)
{
    g_score = n;
}

/* EOF */