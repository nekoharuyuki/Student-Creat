//===============================================================================================================
//
// game.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "main.h"
#include "directX3d.h"
#include "debugproc.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

// �T�E���h�Ɋւ���w�b�_�[�t�@�C��
#include "com.h"
#include "sound.h"
#include "snddata.h"

// �Q�[��
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"
#include "goal.h"
#include "readygo.h"

// �V�[��
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "gameover.h"
#include "gameclear.h"
#include "ranking.h"

// �t�B�[���h�̌`��
#include "dungeon.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// �Q�[���J��
static GAMEMODE g_GameMode = SCENE_TITLE;

// �Q�[���̎��Ԏ擾
static int g_nCountTime = 0;
static int a = 0;
static int b = 0;

// �������t���O
static bool g_bInitTitleFlg      = false;		// �^�C�g��
static bool g_bInitTutorialFlg   = false;		// �`���[�g���A��
static bool g_bInitGameFlg       = false;		// �Q�[��
static bool g_bInitGameOverFlg   = false;		// �Q�[���I�[�o�[
static bool g_bInitGameClearFlg  = false;		// �Q�[���N���A
static bool g_bInitGameRecordFlg = false;		// �Q�[�����R�[�h

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

static bool bOn = false;
static int nTimeTemp = 0;

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

//=============================================================================
//
// �Q�[���̃��C������
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void GameMain(void)
{
	// ��ʑJ��
	switch( g_GameMode )
	{
	// �^�C�g��
	case SCENE_TITLE:

		Title();

		break;

	// �`���[�g���A��
	case SCENE_TUTORIAL:

		Tutorial();

		break;

	// �Q�[��
	case SCENE_GAME:

		Game();

		break;
	
	// �Q�[���I�[�o�[
	case SCENE_GAMEOVER:

		GameOver();

		break;

	// �Q�[���N���A
	case SCENE_CLEAR:

		GameClear();

		break;

	// �Q�[���̋L�^
	case SCENE_RECORD:

		GameRecord();

		break;

	case SCENE_MAX:

		UnInit();

		g_bInitTitleFlg      = false;		// �^�C�g��
		g_bInitTutorialFlg   = false;		// �`���[�g���A��
		g_bInitGameFlg       = false;		// �Q�[��
		g_bInitGameOverFlg   = false;		// �Q�[���I�[�o�[
		g_bInitGameClearFlg  = false;		// �Q�[���N���A
		g_bInitGameRecordFlg = false;		// �Q�[�����R�[�h

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

		bOn = false;
		nTimeTemp = 0;

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

		SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );
		//SetMode( SCENE_TITLE );

		break;
	}
}

//=============================================================================
//
// �Q�[���̃^�C�g������
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void Title(void)
{
	//--------------------
	// ����������
	//--------------------

	if( g_bInitTitleFlg == false )
	{
		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_BGM000 );

		InitTitle();

		g_bInitTitleFlg = true;
	}

	//--------------------

	//--------------------
	// �X�V����
	//--------------------

	UpdateTitle();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	DrawTitle();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_SE003 );

		SetFade( FADE_OUT, FADE_IN, SCENE_TUTORIAL );

		FADE Fade = GetFade();

		if( Fade == FADE_OUT )
		{
			// �^�C�g���̏I������
			UninitTitle();
		}

	}

	//--------------------
}

//=============================================================================
//
// �`���[�g���A������
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void Tutorial(void)
{
	//--------------------
	// ����������
	//--------------------

	if( g_bInitTutorialFlg == false )
	{
		InitTutorial();

		g_bInitTutorialFlg = true;
	}

	//--------------------

	//--------------------
	// �X�V����
	//--------------------

	UpdateTutorial();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	DrawTutorial();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_SE003 );

		StopSnd( SOUND_LABEL_BGM000 );

		SetFade( FADE_OUT, FADE_IN, SCENE_GAME );

		FADE Fade = GetFade();

		if( Fade == FADE_OUT )
		{
			// �`���[�g���A���̏I������
			UninitTutorial();
		}

//		SetMode( SCENE_GAME );
	}

	//--------------------
}

//=============================================================================
//
// �Q�[������
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void Game(void)
{
	//--------------------
	// ����������
	//--------------------

	if( g_bInitGameFlg == false )
	{
		InitReadGo();

		SelectDungeon();

		// �n�ʂ̏�����
		InitField();

		// �ǂ̏�����
		InitWall();

		// �V��̏�����
		InitCeiling();

		// �e�̏�����
		InitShadow();

		// ���f���̏�����
		InitModel();

		// �r���{�[�h�̏�����
		InitBillboard();

		// �����G�t�F�N�g�̏�����
		InitExplosion();

		// �X�R�A�̏�����
		InitNumber();

		// �S�[���̏�����
		InitGoal();

		a = 0;
		b = 0;
		g_nCountTime = 0;

		a = timeGetTime();

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

		nTimeTemp = 0;

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_BGM001 );
		PlaySnd( SOUND_LABEL_SE006 );

		g_bInitGameFlg = true;
	}

	//--------------------

	// ���f���̏��擾
	MODEL *Model = GetModel();

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	bool DebugFlg = DebugProcFlg();
	if( Model -> bGoal == false )
	{
		/* �Q�[���J�n����̌o�ߎ��Ԃ��擾 */
		b = timeGetTime();

		// �L�[���͔���
		if( PushKey( DIK_F2 ) && DebugFlg == true )
		{
			if( bOn == false )
			{	// ���Ԃ��~�߂�
				// �~�߂����̎��ԕۑ�
				nTimeTemp = timeGetTime();
				bOn = true;
			}
			else if( bOn == true )
			{	// ���Ԃ𓮂���
				a = timeGetTime();
				bOn = false;
			}
		}

		// �L�[���͔���
		if( PushKey( DIK_G ) && DebugFlg == true )
		{
			SetMode( SCENE_CLEAR );
		}

		if( bOn )
		{
			g_nCountTime = ( nTimeTemp - a ) / 1000;
		}
		else
		{
			g_nCountTime = ( b - a ) / 1000;
		}

		g_nCountTime = TIME_LIMIT - g_nCountTime;

		// �L�[���͔���
		if( PushKey( DIK_BACKSPACE ) && DebugFlg == true )
		{
			g_nCountTime = 0;
		}
	}

	SetScore( g_nCountTime );

#else if			// �����[�X�̎�

	if( Model->bGoal == false )
	{
		/* �Q�[���J�n����̌o�ߎ��Ԃ��擾 */
		b = timeGetTime();
		g_nCountTime = ( b - a ) / 1000;
		g_nCountTime = TIME_LIMIT - g_nCountTime;
	}

	SetScore( g_nCountTime );

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	//--------------------
	// �X�V����
	//--------------------

	// �n�ʂ̍X�V
	UpdataFiled();

	// �ǂ̍X�V
	UpdataWall();

	// �V��̍X�V
	UpdataCeiling();

	// �e�̍X�V
	UpdataShadow();

	// �r���{�[�h�̍X�V
	UpdataBillboard();

	// �����G�t�F�N�g�̍X�V
	UpdataExplosion();

	// ���f���̍X�V
	UpdataModel();

	// �X�R�A�̍X�V
	UpdateNumber();

	// �S�[���̍X�V
	UpdataGoal();

	// RedyGo�̍X�V
	UpdateReadGo();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	// �n�ʂ̕`��
	DrawField();

	// �ǂ̕`��
	DrawWall();

	// �V��̕`��
	DrawCeiling();

	// �e�̕`��
	DrawShadow();

	// ���f���̕`��
	DrawModel();

	// �r���{�[�h�̕`��
	DrawBillboard();

	// �����G�t�F�N�g�̕`��
	DrawExplosion();

	if( Model->bGoal == false )
	{	// �S�[���֓������Ă��Ȃ��ꍇ

		// �S�[���̕`��
		DrawGoal();

		// �X�R�A�̕`��
		DrawNumber();
	}

	// ReadyGo�̕\��
	DrawReadGo();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	// �Q�[���N���A

	if( Model->bGoal == true )	// �S�[���֓��������ꍇ
	{
		SetMode( SCENE_CLEAR );
	}

	// �Q�[���I�[�o�[

	if( g_nCountTime == 0 )	// �������ԂɂȂ��Ă��܂����ꍇ
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_GAMEOVER );

		// �n�ʂ̏I������
		UnInitField();

		// �ǂ̏I������
		UnInitWall();

		// �V��̏I������
		UnInitCeiling();

		// �r���{�[�h�̏I������
		UnInitBillboard();

		// ���f���̏I������
		UnInitModel();

		// �e�̏I������
		UnInitShadow();

		// �����G�t�F�N�g�̏I������
		UnInitExplosion();

		// �S�[���̏I������
		UnInitGoal();

		UninitReadGo();

		g_bInitGameFlg = false;

		SetMode( SCENE_GAMEOVER );
	}

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O���\��
	if( bOn == true )
	{
		PrintDebugProc("TIME STOP \n");
	}
	else
	{
		PrintDebugProc("TIME MOVE \n");
	}

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	//--------------------
}

//=============================================================================
//
// �Q�[���̎��Ԏ擾
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

int GetGameTime(void)
{
	return g_nCountTime;
}

//=============================================================================
//
// �Q�[���N���A����
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void GameClear( void )
{
	// �Q�[��
	Game();

	//--------------------
	// ����������
	//--------------------

	if( g_bInitGameClearFlg == false )
	{
		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_SE004 );

		InitGameClear();

		g_bInitGameClearFlg = true;		// �Q�[���N���A
	}

	//--------------------

	//--------------------
	// �X�V����
	//--------------------

	UpdateGameClear();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	DrawGameClear();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

//		StopSnd( SOUND_LABEL_BGM001 );

		// �n�ʂ̏I������
		UnInitField();

		// �ǂ̏I������
		UnInitWall();

		// �V��̏I������
		UnInitCeiling();

		// �r���{�[�h�̏I������
		UnInitBillboard();

		// ���f���̏I������
		UnInitModel();

		// �e�̏I������
		UnInitShadow();

		// �����G�t�F�N�g�̏I������
		UnInitExplosion();

		// �S�[���̏I������
		UnInitGoal();

		// �Q�[���N���A�\���̏I������
		UninitGameClear();

		UninitReadGo();

		StopSnd( SOUND_LABEL_BGM001 );

		g_bInitGameClearFlg = false;
		g_bInitGameFlg      = false;

		SetMode( SCENE_RECORD );
	}

	//--------------------
}

//=============================================================================
//
// �Q�[���I�[�o�[����
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void GameOver(void)
{
	//--------------------
	// ����������
	//--------------------

	if( g_bInitGameOverFlg == false )
	{
		StopSnd( SOUND_LABEL_BGM001 );

		// �T�E���h�Đ�
		PlaySnd( SOUND_LABEL_BGM002 );

		InitGameOver();

		g_bInitGameOverFlg = true;
	}

	//--------------------

	//--------------------
	// �X�V����
	//--------------------

	UpdateGameOver();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	DrawGameOver();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

		StopSnd( SOUND_LABEL_BGM002 );

		UninitGameOver();

		// �X�R�A�̏I������
		UninitNumber();

		g_bInitGameOverFlg = false;

		SetMode( SCENE_MAX );
	}

	//--------------------
}

//=============================================================================
//
// �Q�[�����R�[�h����
//
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//=============================================================================

void GameRecord(void)
{
	//--------------------
	// ����������
	//--------------------

	if( g_bInitGameRecordFlg == false )
	{
		InitGameRecord();

		g_bInitGameRecordFlg = true;
	}

	//--------------------

	//--------------------
	// �X�V����
	//--------------------

	UpdateGameRecord();

	SetScore( g_nCountTime );

	// �X�R�A�̍X�V
	UpdateNumber();

	//--------------------

	//--------------------
	// �`�揈��
	//--------------------

	DrawGameRecord();

	// �X�R�A�̕`��
	DrawNumber();

	//--------------------

	//--------------------
	// �I������
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

		UninitGameRecord();

		// �X�R�A�̏I������
		UninitNumber();

		g_bInitGameRecordFlg = false;

		SetMode( SCENE_MAX );
	}

	//--------------------
}

//============================================================================
//
// ���̃Q�[���J�ڗp�ϐ��Z�b�g �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@scene�@�c�@�Q�[���J�� �ϐ�
//
//============================================================================

void SetMode( GAMEMODE next )
{
	g_GameMode = next;
}

GAMEMODE GetMode(void)
{
	return g_GameMode;
}


/* EOF */