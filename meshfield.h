//===============================================================================================================
//
// mashfield.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _FIELD_H_
#define _FIELD_H_

//============================================================================
//
// �}�N����`
//
//============================================================================

// �n�ʂ̃e�N�X�`���t�@�C��
#define FIELD_FILE              ( "data/TEXTURE/field003.jpg" )

#define FIELD_MOVE_SPEED        ( 0.f )		// �n�ʂ̈ړ���
#define TEXTURE_NUM             ( 3 )		// �e�N�X�`����
#define TEXTURE_SIZE            ( 100 )		// �e�N�X�`���̃T�C�Y

//============================================================================
//
// �\���̒�`
//
//============================================================================

/* �Ǘp �\���� */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// �o�[�e�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  pD3DIndxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9      pD3DTexture;		// �e�N�X�`��

	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scl;		// �傫��

	bool bUse;				// �g�p���Ă��邩�ǂ���
}MESH_FIELD;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitField(void);	// �t�B�[���h�̏�����
void DrawField(void);		// �t�B�[���h�̕`�揈��
void UpdataFiled(void);		// �t�B�[���h�̍X�V����
void UnInitField(void);		// �t�B�[���h�̊J������

MESH_FIELD GetField( int nIdx );	// �t�B�[���h�̏��擾

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _FIELD_H_