//===============================================================================================================
//
// explosion.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "billboard.h"

//============================================================================
//
// �}�N����`
//
//============================================================================

// �n�ʂ̃e�N�X�`���t�@�C��
#define EXPLOSION_FILE              ( "data/TEXTURE/Special2.png" )
#define TEX_PATTERN_X               ( 5 )
#define TEX_PATTERN_Y               ( 3 )

// �e�N�X�`���̃T�C�Y
#define EXPLOSION_SIZE              ( 80.f )	// ����

// �����̐�
#define EXPLOSION_NUM               ( 1 + BILLBOARD_NUM )

//============================================================================
//
// �\���̒�`
//
//============================================================================

/* �Ǘp �\���� */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// �o�[�e�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9      pD3DTexture;		// �e�N�X�`��

	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scl;		// �傫��

	int nAnimeNo;
	int nAnimeTime;

	bool bUse;
}EXPLOSION;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitExplosion(void);	// �����G�t�F�N�g�̏����� ����
void DrawExplosion(void);		// �����G�t�F�N�g�̕`�� ����
void UpdataExplosion(void);		// �����G�t�F�N�g�̍X�V ����
void UnInitExplosion(void);		// �����G�t�F�N�g�̊J�� ����

void SetTextureAnime(int animeNo);

EXPLOSION* GetExplosion(void);	// �����G�t�F�N�g�̏��擾 ����

void SetExplosion( D3DXVECTOR3 pos );

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _EXPLOSION_H_