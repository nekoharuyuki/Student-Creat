//===============================================================================================================
//
// input.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* �C���N���[�h�K�[�h�̐擪 */
#ifndef _INPUT_H_
#define _INPUT_H_

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION ( 0x0800 )	// �x�����ꎞ�I�ɖ����ɂ���
#endif

#include <dinput.h>

//============================================================================
//
// �֐��v���g�^�C�v�錾
//
//============================================================================

HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd);		// DirectInput �������֐�
void ReleaseDInput(void);								// DirectInput�f�o�C�X�ƃI�u�W�F�N�g�̊J�� �֐�
void UpdataDInput(void);								// DirectInput �X�V�֐�
bool nowKey(BYTE cKey);									// �L�[���͔���
bool PushKey(BYTE cKey);								// �L�[���͔���i �������u�� �j
bool ReleaseKey(BYTE cKey);								// �L�[���͔���i �������u�� �j

/* �C���N���[�h�K�[�h�̍ŏI�s */
#endif	// _INPUT_H_