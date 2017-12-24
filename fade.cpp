//===============================================================================================================
//
// fade.cpp
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
#include "input.h"
#include "fade.h"
#include "game.h"

//============================================================================
//
// �}�N����`
//
//============================================================================

#define	FADE_RATE		(0.05f)		// �t�F�[�h�W��

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT MakeVertexFade(void);
void SetColor(D3DCOLOR col);

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static LPDIRECT3DTEXTURE9      g_p3DTextureFade  = NULL;	// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFade = NULL;	// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
static D3DXCOLOR               g_color;

static FADE					   g_fade     = FADE_IN;
static FADE                    g_fadeNext = FADE_IN;

static GAMEMODE                g_nextGameMode = SCENE_TITLE;

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	 S_OK	: �������ɐ���
//	 E_FAIL	: �������Ɏ��s
//
//============================================================================

HRESULT InitFade(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_fade     = FADE_IN;
	g_fadeNext = FADE_IN;

	g_nextGameMode = SCENE_TITLE;

	// ���_���̐ݒ�
	MakeVertexFade();

	HRESULT hr;

	// �e�N�X�`���̓ǂݍ���
	hr = D3DXCreateTextureFromFile( D3DDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_FADE_FILE,		// �t�@�C���̖��O
									&g_p3DTextureFade );	// �ǂݍ��ރ������[

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

	return S_OK;
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

void UninitFade(void)
{
	// �e�N�X�`���̊J��
	if( g_p3DTextureFade != NULL )
	{
		g_p3DTextureFade -> Release();
	}
	// ���_�o�b�t�@�̊J��
	if( g_pD3DVtxBuffFade != NULL )
	{
		g_pD3DVtxBuffFade -> Release();
	}
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

void UpdateFade(void)
{
    switch( g_fade )
    {
    case FADE_NONE:
        g_color.a = 0.0f;
        break;
    case FADE_IN:
    case BLACK_IN:
		g_color.a -= FADE_RATE;		// ���l�����Z���ĉ�ʂ𕂂��オ�点��
		if( g_color.a <= 0.0f )
		{
			// �t�F�[�h�����I��
			g_color.a = 0.0f;
			g_fade = FADE_NONE;
		}
        break;
    case FADE_OUT:
    case BLACK_OUT:
		g_color.a += FADE_RATE;		// ���l�����Z���ĉ�ʂ������Ă���
		if( g_color.a >= 1.0f )
		{
			// �t�F�[�h�C�������ɐ؂�ւ�
			g_color.a = 1.0f;
			g_fade = g_fadeNext;

			// ���[�h��ݒ�
			SetMode( g_nextGameMode );
		}
        break;
    }
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

void DrawFade(void)
{
	if(g_fade != FADE_NONE)
	{
		// DirectX�f�o�C�X�̒l���擾
		LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

		switch(g_fade)
		{
			case BLACK_IN:
			case BLACK_OUT:
				D3DDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
				break;
		}

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		D3DDevice -> SetStreamSource(0, g_pD3DVtxBuffFade, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		D3DDevice -> SetFVF( FVF_VERTEX_2D );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture(0, NULL);

		// �|���S���̕`��
		D3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
		switch(g_fade)
		{
			case BLACK_IN:
			case BLACK_OUT:
        		D3DDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
				break;
		}
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

HRESULT MakeVertexFade(void)
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if( FAILED( D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												 D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g�p�@�@
												 FVF_VERTEX_2D,				// �g�p���钸�_�t�H�[�}�b�g
												 D3DPOOL_MANAGED,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												 &g_pD3DVtxBuffFade,		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												 NULL)))					// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFade -> Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx[2].vtx = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	// �e�N�X�`���̃p�[�X�y�N�e�B�u�R���N�g�p
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_color = D3DXCOLOR( 0.f, 0.f, 0.f, 0.01f );

	pVtx[0].diffuse = g_color;
	pVtx[1].diffuse = g_color;
	pVtx[2].diffuse = g_color;
	pVtx[3].diffuse = g_color;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f );	

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffFade -> Unlock();

	return S_OK;
}

//============================================================================
//
// �F��ݒ� �֐�
// �����F
//�@D3DCOLOR col
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void SetColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFade->Lock( 0, 0, (void**)&pVtx, 0 );

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffFade -> Unlock();
}

//============================================================================
//
// �t�F�[�h�̏�Ԑݒ� �֐�
// �����F
//�@FADE out
//  FADE in
//  GAMEMODE next
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void SetFade( FADE out, FADE in, GAMEMODE next )
{
	g_fade         = out;
	g_fadeNext     = in;
	g_nextGameMode = next;
}

//============================================================================
//
// �t�F�[�h�̏�Ԏ擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_fade
//
//============================================================================

FADE GetFade(void)
{
	return g_fade;
}