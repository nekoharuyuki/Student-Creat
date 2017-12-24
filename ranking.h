//===============================================================================================================
//
// ranking.h
// author : Haruyuki Neko
//
//===============================================================================================================

// �摜�S�ʃw�b�_�[

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _GAME_RANKING_H_
#define _GAME_RANKING_H_

//============================================================================
//
// �}�N����`
//
//============================================================================

// �e�N�X�`���t�@�C����
#define TEXTURE_RANKING_FILE        "data/TEXTURE/GameRanking.png"
#define TEXTURE_SECOND_FILE			"data/TEXTURE/GameSecond.png"

#define TEX_RANKING_SIZE_X          ( 10 )
#define TEX_RANKING_SIZE_Y          ( 10 )

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitGameRecord(void);		// ������ �֐�
void UpdateGameRecord(void);		// �X�V���� �֐�
void DrawGameRecord(void);		// �`�揈�� �֐�
void UninitGameRecord(void);		// �J������ �֐�

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _GAME_RANKING_H_