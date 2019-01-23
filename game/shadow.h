//===============================================================================================================
//
// shadow.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _SHADOW_H_
#define _SHADOW_H_

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
#define SHADOW_FILE              ( "data/TEXTURE/shadow000.jpg" )

// �e�N�X�`���̃T�C�Y
#define SHADOW_SIZE_MODEL              ( 40.f )	// ���f��
#define SHADOW_SIZE_BILLBOARD          ( 8.f )	// �r���{�[�h
#define SHADOW_SIZE_GOAL               ( 50.f )	// �r���{�[�h(�S�[��)

// �e�̎��
#define MAX_SHADOW                     ( 1 + BILLBOARD_NUM )	// �ő吔

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

	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scl;		// �傫��

	bool bUse;
	float fsize;
}SHADOW;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitShadow(void);	// �V���h�E�̏�����
void DrawShadow(void);		// �V���h�E�̕`�揈��
void UpdataShadow(void);	// �V���h�E�̍X�V����
void UnInitShadow(void);	// �V���h�E�̊J������

int SetShadow(D3DXVECTOR3 pos, float size);
void ResetShadow(int nNumber, D3DXVECTOR3 pos);
void DestroyShadow(int nNumber);

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _SHADOW_H_