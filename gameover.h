//===============================================================================================================
//
// gameover.h
// author : Haruyuki Neko
//
//===============================================================================================================

// �摜�S�ʃw�b�_�[

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

//============================================================================
//
// �}�N����`
//
//============================================================================

// �e�N�X�`���t�@�C����
#define TEXTURE_GAMEOVER_FILE        "data/TEXTURE/GameOver.png"
#define TEXTURE_CRYPIG_FILE	         "data/TEXTURE/pig_2.png"
#define TEXTURE_CRY_FILE	         "data/TEXTURE/GameOverTear.png"

#define TEX_PIG_SIZE                 ( 200 )

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitGameOver(void);		// ������ �֐�
void UpdateGameOver(void);		// �X�V���� �֐�
void DrawGameOver(void);		// �`�揈�� �֐�
void UninitGameOver(void);		// �J������ �֐�

void Rotation(float Rota);

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _GAME_OVER_H_