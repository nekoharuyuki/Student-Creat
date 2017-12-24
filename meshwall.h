//===============================================================================================================
//
// mashfield.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _WALL_H_	// �C���N���[�h�K�[�h�J�n
#define _WALL_H_	// �C���N���[�h�K�[�h�p�}�N����`

//============================================================================
//
// �}�N����`
//
//============================================================================

// �n�ʂ̃e�N�X�`���t�@�C��
#define WALL_FILE              ( "data/TEXTURE/wall000.jpg" )
#define WALL_SKY               ( "data/TEXTURE/sky001.jpg" )

#define MAX_WALL               ( 4 )
#define MAX_SKY                ( 3 )

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

	int nNumBlockX, nNumBlockY;
	int nNumVertex;
	int nNumIndex;
	int nNumPolygon;
	float fSizeBlockX, fSizeBlockY;
	bool bUse;
}MESH_WALL;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitWall(void);		// �E�H�[���̏�����
void DrawWall(void);		// �E�H�[���̕`�揈��
void UpdataWall(void);		// �E�H�[���̍X�V����
void UnInitWall(void);		// �E�H�[���̊J������

// �E�H�[���ݒ�p
HRESULT InitWallUp(void);
void UpdataWallUp(void);
void DrawWallUp(void);
void UnInitWallUp(void);

HRESULT InitWallDown(void);
void UpdataWallDown(void);
void DrawWallDown(void);
void UnInitWallDown(void);

HRESULT InitWallLeft(void);
void UpdataWallLeft(void);
void DrawWallLeft(void);
void UnInitWallLeft(void);

HRESULT InitWallRight(void);
void UpdataWallRight(void);
void DrawWallRight(void);
void UnInitWallRight(void);

// ���E�̐ݒ�p
HRESULT InitSky(void);
void UpdataSky(void);
void DrawSky(void);
void UnInitSky(void);

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _WALL_H_