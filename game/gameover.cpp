//===============================================================================================================
//
// gameover.cpp
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
#include "gameover.h"
#include "title.h"

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

void MakeVertexGameOver(void);

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureGameOver = NULL;		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffGameOver = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

static LPDIRECT3DTEXTURE9		g_pD3DTextureCryPig   = NULL;		// �e�N�X�`���ւ̃|�C���^�i�u�^�j
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffCryPig   = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�i�u�^�j

static LPDIRECT3DTEXTURE9		g_pD3DTextureTear     = NULL;		// �e�N�X�`���ւ̃|�C���^�i�܁j
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTear     = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^�i�܁j

static LPDIRECT3DTEXTURE9		g_pD3DTextureEnter    = NULL;		// �e�N�X�`���ւ̃|�C���^(Enter�p)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffEnter    = NULL;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^(Enter�p)

static D3DXVECTOR2 g_CryPig;				// �L�����N�^�[�̃|�W�V����

static float g_fRadian;			// ��]
static float g_fR;				// ���a
static float g_fCenter_X;		// ���S�_X
static float g_fCenter_Y;		// ���S�_Y
static float g_fRota;			// ��]��

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@HRESULT hr
//
//============================================================================

HRESULT InitGameOver(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ��]�̏�����
	g_fRadian   = 0.f;	// ��]
	g_fR        = 0.f;	// ���a
	g_fCenter_X = 0.f;	// ���S�_X
	g_fCenter_Y = 0.f;	// ���S�_Y
	g_fRota     = 0.f;	// ��]��

	g_fRadian = atan2f( TEX_PIG_SIZE, TEX_PIG_SIZE );
	g_fR = sqrtf( ( TEX_PIG_SIZE * TEX_PIG_SIZE ) + ( TEX_PIG_SIZE * TEX_PIG_SIZE ) );

	g_CryPig = D3DXVECTOR2(0.0f, 0.0f);
	g_fCenter_X = g_CryPig.x + SCREEN_WIDTH  / 2;	// ���S�_X
	g_fCenter_Y = g_CryPig.y + SCREEN_HEIGHT / 2;	// ���S�_Y

	// �L�����N�^�[�̃|�W�V����
	g_CryPig = D3DXVECTOR2( g_fCenter_X, g_fCenter_Y );

	// ���_���̍쐬
	MakeVertexGameOver();

	HRESULT hr;

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice,					// �f�o�C�X�ւ̃|�C���^
									TEXTURE_GAMEOVER_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureGameOver);	// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,					// �f�o�C�X�ւ̃|�C���^
									TEXTURE_CRYPIG_FILE,		// �t�@�C���̖��O
									&g_pD3DTextureCryPig);		// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,					// �f�o�C�X�ւ̃|�C���^
									TEXTURE_CRY_FILE,			// �t�@�C���̖��O
									&g_pD3DTextureTear);		// �ǂݍ��ރ������[

	hr = D3DXCreateTextureFromFile( D3DDevice,					// �f�o�C�X�ւ̃|�C���^
									TEXTURE_ENTER_FILE,			// �t�@�C���̖��O
									&g_pD3DTextureEnter);		// �ǂݍ��ރ������[

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

void MakeVertexGameOver(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffGameOver,	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffCryPig,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffTear,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
									 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
									 FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
									 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
									 &g_pD3DVtxBuffEnter,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
									 NULL);						// NULL�ɐݒ�

	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffGameOver -> Lock( 0, 0, (void**)&pVtx, 0 );

	// ���W�w��
	pVtx[0].vtx = D3DXVECTOR3(0, 0, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffGameOver -> Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffCryPig -> Lock( 0, 0, (void**)&pVtx, 0 );

	// ���W�w��
	pVtx[0].vtx = D3DXVECTOR3(g_CryPig.x - TEX_PIG_SIZE, g_CryPig.y - TEX_PIG_SIZE, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(g_CryPig.x + TEX_PIG_SIZE, g_CryPig.y - TEX_PIG_SIZE, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(g_CryPig.x - TEX_PIG_SIZE, g_CryPig.y + TEX_PIG_SIZE, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(g_CryPig.x + TEX_PIG_SIZE, g_CryPig.y + TEX_PIG_SIZE, 0.f);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffCryPig -> Unlock();

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffTear -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffTear -> Unlock();

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

void UpdateGameOver(void)
{
/*
	// ��]����
	if( g_fRota > D3DXToRadian( 135 ) )
	{
		g_fRota -= 0.02f;			   // ��]
		Rotation( g_fRota );
	}

	if( g_fRota < D3DXToRadian( 225 ) )
	{
		g_fRota += 0.02f;			   // ��]
		Rotation( g_fRota );
	}
*/
#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O���\��
	PrintDebugProc( "PIG POS : ( %.2f : %.2f )\n", g_CryPig.x, g_CryPig.y );
	PrintDebugProc( "PIG ROT : ( %.2f )\n", g_fRota );

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

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

void DrawGameOver(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffGameOver, 0, sizeof( VERTEX_2D ) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	D3DDevice -> SetTexture( 0, g_pD3DTextureGameOver );

	// �|���S���̕`��
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffCryPig, 0, sizeof( VERTEX_2D ) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	D3DDevice -> SetTexture( 0, g_pD3DTextureCryPig );

	// �|���S���̕`��
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	int nTime = GetTime();

	if( nTime % 2 == 0 )
	{
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
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTear, 0, sizeof( VERTEX_2D ) );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_pD3DTextureTear );

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

void UninitGameOver(void)
{
	// �e�N�X�`���̊J��
	if( g_pD3DTextureGameOver != NULL )
	{
		g_pD3DTextureGameOver -> Release();
		g_pD3DTextureGameOver = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffGameOver != NULL )
	{
		g_pD3DVtxBuffGameOver -> Release();
		g_pD3DVtxBuffGameOver = NULL;
	}

	// �e�N�X�`���̊J��
	if( g_pD3DTextureTear != NULL )
	{
		g_pD3DTextureTear -> Release();
		g_pD3DTextureTear = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffTear != NULL )
	{
		g_pD3DVtxBuffTear -> Release();
		g_pD3DVtxBuffTear = NULL;
	}

	// �e�N�X�`���̊J��
	if( g_pD3DTextureCryPig != NULL )
	{
		g_pD3DTextureCryPig -> Release();
		g_pD3DTextureCryPig = NULL;
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffCryPig != NULL )
	{
		g_pD3DVtxBuffCryPig -> Release();
		g_pD3DVtxBuffCryPig = NULL;
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

//============================================================================
//
// �e�N�X�`���̓���(��]) �֐�
// �����F
//�@float Rota
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void Rotation(float Rota)
{
	VERTEX_2D *pVtx;

	g_pD3DVtxBuffCryPig -> Lock( 0 ,0 , ( void** )&pVtx , 0 );

	g_CryPig.x = g_fCenter_X - sinf( g_fRadian - g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y - cosf( g_fRadian - g_fRota ) * g_fR;

	pVtx[0].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X + sinf( g_fRadian + g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y - cosf( g_fRadian + g_fRota ) * g_fR;

	pVtx[1].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X - sinf( g_fRadian + g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y + cosf( g_fRadian + g_fRota ) * g_fR;

	pVtx[2].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X + sinf( g_fRadian - g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y + cosf( g_fRadian - g_fRota ) * g_fR;

	pVtx[3].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_pD3DVtxBuffCryPig -> Unlock();
}

/* EOF */