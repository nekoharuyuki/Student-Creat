//===============================================================================================================
//
// main.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _MAIN_H_	// �C���N���[�h�K�[�h�J�n
#define _MAIN_H_	// �C���N���[�h�K�[�h�p�}�N����`

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#define STRICT					// �^�`�F�b�N�������ɍs��
#define D3D_DEBUG_INFO			// Direct3D�f�o�b�O���̗L����
//#define WIN32_LEAN_AND_MEAN	// �w�b�^�[���炠�܂�g���Ȃ��֐����Ȃ�

// Windows �֘A
#include <windows.h>			// windows�w�b�^�[

// �W��	�֘A
#include <stdio.h>
#include <string.h>
#include <Math.h>
#include <time.h>

// DirectX �֘A
#include <d3dx9.h>		// Direct3D & D3DX�i �����Łud3d9.h�v���C���N���[�h �j
#include <dxerr.h>		// DirectX�̃G���[�������[�e�B���e�B�E���C�u����

//============================================================================
//
// �}�N����`
//
//============================================================================

/* �E�C���h�E�̖��O */
#define CLASS_NAME      "�E�C���h�E�N���X"		// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME     "�_���W�����Q�[��"		// �E�C���h�E�̃L���v�V������

/* �E�C���h�E�T�C�Y */
#define SCREEN_WIDTH    ( 940 )	// �E�C���h�E�̕�
#define SCREEN_HEIGHT   ( 680 )	// �E�C���h�E�̍���

/* 2D�p�̒��_�t�H�[�}�b�g */
#define	FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
/* 3D�p�̒��_�t�H�[�}�b�g */
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

//============================================================================
//
// �\���̒�`
//
//============================================================================

/* �e�N�X�`���p �\���� */

// ���_�\����
// ��L�t�H�[�}�b�g�ɍ��킹���\����

/* 2D */
typedef struct
{
	D3DXVECTOR3 vtx;		// 3D�̒��_���W�i X, Y �j
	float		rhw;
	D3DCOLOR    diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
}VERTEX_2D;

/* 3D */
typedef struct
{
	D3DXVECTOR3 vtx;		// 3D�̒��_���W�i X, Y, Z �j
	D3DXVECTOR3 nor;		// �@���x�N�g���i �ʂ̌��� �j
	D3DCOLOR    diffuse;	// ���ˌ�
	D3DXVECTOR2 tex;		// �e�N�X�`�����W
}VERTEX_3D;

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT Init(void);		// ������
void Draw(void);		// �`�揈��
void Updata(void);		// �X�V����
void UnInit(void);		// �J������

int GetTime(void);	// ���Ԏ擾

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _MAIN_H_