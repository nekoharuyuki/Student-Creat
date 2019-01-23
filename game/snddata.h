//=============================================================================================================
//
// �T�E���h�f�[�^���܂Ƃ߂��R�[�h�t�@�C��
// ����ҁF���q ���s
//
//=============================================================================================================
#ifndef _SNDDATA_H_	// �C���N���[�h�K�[�h�J�n
#define _SNDDATA_H_	// �C���N���[�h�K�[�h�p�}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "sound.h"

//*****************************************************************************
// �񋓑̒�`
//*****************************************************************************
// �T�E���h�t�@�C���h�c
typedef enum
{
	SOUND_LABEL_ST  = 0,								// �h�c�̐擪
	SOUND_LABEL_BEF = SOUND_LABEL_ST  - 1,				// �h�c�̐擪�O

	SOUND_LABEL_BGM000,									// �T���v��BGM(WAV�t�@�C��)
	SOUND_LABEL_BGM001,									// �T���v��BGM(WAV�t�@�C��)
	SOUND_LABEL_BGM002,									// �T���v��BGM(WAV�t�@�C��)
	SOUND_LABEL_SE000,									// �T���v��SE
	SOUND_LABEL_SE001,									// �T���v��SE
	SOUND_LABEL_SE002,									// �T���v��SE
	SOUND_LABEL_SE003,									// �T���v��SE
	SOUND_LABEL_SE004,									// �T���v��SE
	SOUND_LABEL_SE005,									// �T���v��SE
	SOUND_LABEL_SE006,									// �T���v��SE
	SOUND_LABEL_SE007,									// �T���v��SE

	SOUND_LABEL_AFT,									// �h�c�̃��X�g��
	SOUND_LABEL_NUM = SOUND_LABEL_AFT - SOUND_LABEL_ST,	// �h�c�̃��X�g
	SOUND_LABEL_ED  = SOUND_LABEL_AFT - -1,				// �h�c�̐�
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//-------------------------------------
// �T�E���h�t�@�C���p�����[�^�擾�֐�
//�����F	SOUND_LABEL			id		�c	[in]	�w��T�E���h�t�@�C���h�c
//�ߒl�F	SOUNDFILEPARAM	*			�c			�w��T�E���h�t�@�C���h�c�̃p�����[�^�|�C���^
//-------------------------------------
SOUNDFILEPARAM	*GetSoundFileParam( SOUND_LABEL id );

#endif//_SNDDATA_H_	// �C���N���[�h�K�[�h�I��