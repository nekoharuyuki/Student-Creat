//===============================================================================================================
//
// billboard.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//============================================================================
//
// �}�N����`
//
//============================================================================

// �r���{�[�h�̃e�N�X�`���t�@�C��
#define BILLBOARD_FILE              ( "data/TEXTURE/bullet000.png" )

#define BILLBOARD_MOVE_SPEED        ( 10.f )	// �r���{�[�h�̈ړ���
#define BILLBOARD_SIZE              ( 10 )		// �e�N�X�`���̃T�C�Y
#define BILLBOARD_NUM               ( 15 )		// �r���{�[�h�̐�

//============================================================================
//
// �\���̒�`
//
//============================================================================

/* �r���{�[�h�p �\���� */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// �o�[�e�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9      pD3DTexture;		// �e�N�X�`��

	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scl;		// �傫��

	bool bUse;

	int nShadowNumber;
	float fShadowSize;

	D3DXVECTOR3 Move;
}BILLBOARD;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitBillboard(void);	// �r���{�[�h�̏�����
void DrawBillboard(void);		// �r���{�[�h�̕`�揈��
void UpdataBillboard(void);		// �r���{�[�h�̍X�V����
void UnInitBillboard(void);		// �r���{�[�h�̊J������

BILLBOARD* GetBillboard(void);			// �r���{�[�h�̈ʒu�擾
void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot );

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _BILLBOARD_H_