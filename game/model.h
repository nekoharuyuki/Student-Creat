//===============================================================================================================
//
// model.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _MODEL_H_
#define _MODEL_H_

//============================================================================
//
// �}�N����`
//
//============================================================================

// ���f��
#define MODEL_FILE           ( "data/MODEL/pig.x" )			// X�t�@�C��
#define MODEL_TEXTURE_FILE   ( "data/MODEL/wing.png" )
#define MODEL_MOVE_SPEED     ( 3.f )						// ���f���̈ړ���
#define MODEL_SIZE           ( 50.0f )

// �s��
#define	MOVE_SPD_ACL      ( 0.4f )	// �ړ������x
#define	MOVE_SPD_ACL_AIR  ( 0.75f )	// �ړ������x�i�󒆁j
#define	MOVE_SPD_MAX	  ( 7.f )	// �ړ��ō����x
#define MOVE_SPD_MAX_AIR  ( 10.f )	// �ړ��ō����x�i�󒆁j
#define VALUE_JUMP        ( 15.f )	// �W�����v��
#define VALUE_RESIST      ( 0.5f )	// �ړ����̒�R�i�󒆁j
#define VALUE_GRAVITY     ( 0.06f )	// �d�́iG�j

//============================================================================
//
// �\���̒�`
//
//============================================================================

/* ���f���p �\���� */
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X

	LPDIRECT3DTEXTURE9  pD3DTextureModel;	// ���f���̃e�N�X�`��
	LPD3DXMESH          pMeshModel;			// ���f���̃��b�V�����̃|�C���^
	LPD3DXBUFFER        pD3DXMatBuff;		// ���f���̃}�e���A�����̃|�C���^
	DWORD               nNumMat;			// �}�e���A�����̑���

	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 oldpos;		// �O��̈ʒu
	D3DXVECTOR3 rot;		// ����
	D3DXVECTOR3 scl;		// �傫��

	// ����
	float fAccelerationUp;		// ��
	float fAccelerationDown;	// ��
	float fAccelerationLeft;	// ��
	float fAccelerationRight;	// �E

	// �W�����v
	float fValueResist;		// �ړ����̒�R��ϐ������
	float fValueJump;       // �W�����v�ʂ�ϐ������
	float fValueGravity;   	// �d�́iG�j��ϐ������
	bool bJump;				// �W�����v�t���O

	// ���������p�x
	float fDirection;		// ������������

	// �e
	int nShadowNumber;
	float fShadowSize;

	// �S�[�����Ă���
	bool bGoal;

}MODEL;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitModel(void);	// ���f���̏�����
void UpdataModel(void);		// ���f���̍X�V����
void DrawModel(void);		// ���f���̕`�揈��
void UnInitModel(void);		// ���f���̊J������

MODEL* GetModel(void);		// ���f���̏��擾
D3DXMATRIX* CalcLookAtMatrix( D3DXMATRIX* pout, D3DXVECTOR3* pPos, D3DXVECTOR3* pLook, D3DXVECTOR3* pUp );

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _MODEL_H_