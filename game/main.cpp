//===============================================================================================================
//
// main.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// �C���N���[�h
//
//============================================================================

#include "main.h"

// �V�X�e��
#include "directX3d.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"

// �C���v�b�g�Ɋւ���w�b�_�[�t�@�C��
#include "input.h"
#include "mouse.h"

// �T�E���h�Ɋւ���w�b�_�[�t�@�C��
#include "com.h"
#include "sound.h"
#include "snddata.h"
#include "wavefile.h"

// �Q�[��
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"
#include "goal.h"

// �V�[��
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "gameover.h"
#include "gameclear.h"
#include "readygo.h"

// �t�B�[���h�̌`��
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static int g_nCountFPS;		// FPS�̃J�E���g
static int g_nTime;			// ����

//============================================================================
//
// �E�C���h�E�v���V�[�W���֐��i�R�[���o�b�N�֐��j
// �����F
//�@hWnd    �c  �E�C���h�E�̃n���h��
//�@uMsg    �c  ���b�Z�[�W�̎��ʎq�iWM_~�j
//�@wParam  �c  ���b�Z�[�W�̍ŏ��̃p�����[�^
//�@lParam  �c  ���b�Z�[�W�̔Ԗڂ̃p�����[�^
// �߂�l�F
//�@���b�Z�[�W�̏�������
//
//============================================================================

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
//	case WM_SETCURSOR:
//			SetCursor( NULL );								// Windows�̕W���J�[�\��������
//			return 1;
	case WM_CREATE:	// CreateWindow()�ďo��
		break;
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_ESCAPE:										// [ESC]�L�[�������ꂽ
/*
			if( MessageBox( NULL, "�{���ɏI�����܂����H", "�m�F", MB_YESNO ) == IDNO )	// ���b�Z�[�W�{�b�N�X
			{	// �L�����Z����I��
				return 0; // ���Ȃ����߂ɂ�DefWindowProc()�ɏ��������Ă͂Ȃ�Ȃ�
			}
*/
			DestroyWindow( hWnd );							// �E�C���h�E��j������悤�w������
			break;
		}
		break;
	case WM_CLOSE:    // �E�B���h�E�i���̓A�v���P�[�V�����j���I�����Ȃ���΂Ȃ�Ȃ���
/*
		if( MessageBox( NULL, "�{���ɏI�����܂����H", "�m�F", MB_YESNO ) == IDNO )	// ���b�Z�[�W�{�b�N�X
		{	// �L�����Z����I��
			return 0; // ���Ȃ����߂ɂ�DefWindowProc()�ɏ��������Ă͂Ȃ�Ȃ�
		}
*/
		break;  // ���ėǂ��̂Ȃ�DefWindowProc()�ɂ܂�����Ηǂ�
	case WM_DESTROY:	// �i����{�^�������������āj�E�C���h�E�j����
		PostQuitMessage( 0 );	// "WM_OUIT"���b�Z�[�W��Ԃ�
		break;
	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );	// ����̏������
}

//============================================================================
//
// ���C���֐�
// �����F
//�@hInstance     �c ���̃v���O���������ʂ��邽�߂̃n���h��
//�@hPrevInstance �c ���NULL
//�@lpCmdLine     �c �R�}���h���C������󂯎��������
//�@nCmdShow      �c �A�v���P�[�V�����̏����\�����@���w��
// �߂�l�F
//�@�I���R�[�h
//
//============================================================================

int APIENTRY WinMain( HINSTANCE	hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HRESULT hr;

	// �t���[���J�E���g�p�ϐ�
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	timeBeginPeriod( 1 );	// �n�[�h�E�F�A�^�C�}�[���x���������iWindows�p�̊֐��j

	dwExecLastTime = dwFPSLastTime = timeGetTime();	// ���ݎ����̎擾�iWindows�p�̊֐��j
	dwCurrentTime = dwFrameCount = 0;

	WNDCLASSEX wcex =
	{
		sizeof( WNDCLASSEX ),       	// WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,                 	// �\������E�C���h�E�̃X�^�C�����w��
		WndProc,			            // �E�C���h�E�v���V�[�W���̃A�h���X���w��
		0,                          	// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,                          	// �ʏ�͎g�p���Ȃ��̂�"0"���w��
		hInstance,                  	// WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
		NULL,                       	// �g�p����A�C�R�����w��iWindows�������Ă���A�C�R�����g���Ȃ�NULL�j
		LoadCursor( NULL, IDC_ARROW ),	// �}�E�X�J�[�\�����w��
		( HBRUSH )( COLOR_WINDOW + 1 ),	// �E�C���h�E�̃N���C�A���g�̈�̔w�i�F���w��
		NULL,                       	// Window�ɂ��郁�j���[��ݒ�
		CLASS_NAME,                 	// �E�C���h�E�N���X�̖��O
		NULL                        	// �������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};
	
	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx( &wcex );
	
	// �E�C���h�E���쐬
	HWND hWnd;
	hWnd = CreateWindow( CLASS_NAME,      		// �E�C���h�E�N���X�̖��O
						 WINDOW_NAME,		 	// �E�C���h�E�̖��O
						 WS_OVERLAPPEDWINDOW,	// �E�C���h�E�X�^�C��
						 CW_USEDEFAULT,			// �E�C���h�E�̍���w���W
						 CW_USEDEFAULT,   		// �E�C���h�E�̍���x���W
						 SCREEN_WIDTH,    		// �E�C���h�E�̕�
						 SCREEN_HEIGHT,			// �E�C���h�E�̍���
						 NULL,					// �e�E�C���h�E�̃n���h��
						 NULL,					// ���j���[�n���h���܂��͎q�E�C���h�EID
						 hInstance,				// �C���X�^���X�n���h��
						 NULL );			  	// �E�C���h�E�쐬�f�[�^

	// DirectX�̏�����
	hr = InitDX( hWnd );

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	D3DDevice -> SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// �J�����O���s��
	D3DDevice -> SetRenderState( D3DRS_ZENABLE, TRUE );						// Z�o�b�t�@���g�p
	D3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			// ���u�����h���g�p
	D3DDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// ���\�[�X�J���[�̎w��
	D3DDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// ���f�B�X�e�B�l�[�V�����J���[�̎w��
	
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	D3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );	// �e�N�X�`��U�l�̌J�Ԑݒ�
	D3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );	// �e�N�X�`��V�l�̌J�Ԑݒ�
	D3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// �e�N�X�`���g�厞�̕�Ԑݒ�
	D3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// �e�N�X�`���g�����̕�Ԑݒ�

	// �E�C���h�E�̕\��
	ShowWindow( hWnd, nCmdShow );	// �w�肳�ꂽ�E�C���h�E�̕\����Ԃ�ݒ�i�E�C���h�E��\���j
	UpdateWindow( hWnd );       	// �E�C���h�E�̏�Ԃ𒼂��ɔ��f�i�E�C���h�E�̃N���C�A���g�̈��`�ʁj

	//--------------------
	// �e�평��������
	//--------------------

	// �e�평����
	hr = Init();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

	// DirectInput�Ɋւ��鏉����
	hr = InitDInput( hInstance, hWnd );	// �L�[�{�[�h

	if( FAILED( hr ) )
	{
		// �L�[�{�[�h���ڑ�����Ă��܂���B
	}

	hr = InitDMouse( hInstance, hWnd );	// �}�E�X

	if( FAILED( hr ) )
	{
		// �}�E�X���ڑ�����Ă��܂���B
	}

	// COM������
	InitCOM();

	// �T�E���h����������
	InitSnd( GetSoundFileParam( SOUND_LABEL_ST ), SOUND_LABEL_NUM );

	//--------------------

	// ���b�Z�[�W���[�v�i���C�����[�v�j
	MSG msg;

	int a = timeGetTime();

	while( 1 )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( WM_QUIT == msg.message )
			{	// PeekMessage()���Ă΂ꂽ��
				break;	// ���[�v�I��
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage( &msg );	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage( &msg );	// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			// �t���[���J�E���g
			dwCurrentTime = timeGetTime();
		
			if( ( dwCurrentTime - dwFPSLastTime ) >= 500 )
			{
				g_nCountFPS = dwFrameCount * 1000 / ( dwCurrentTime - dwFPSLastTime );
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
			{
				dwExecLastTime = dwCurrentTime;

				// �o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
				D3DDevice -> Clear( 0,
									NULL,
									( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ),
									D3DCOLOR_RGBA( 0, 0, 0, 0 ),			// �w�i�̐F �ݒ�
									1.0f,
									0 );
				
				hr = D3DDevice -> BeginScene();

				if( SUCCEEDED( hr ) )
				{
					// �J�n����̎��Ԏ擾
					int b = timeGetTime();
					g_nTime = ( b - a ) / 1000;

					//--------------------
					// �e��X�V����
					//--------------------

					// DirectInput�̍X�V����
					UpdataDInput();	// �L�[�{�[�h
					UpdataDMouse(hWnd);	// �}�E�X

					Updata();

					//--------------------
					
					//--------------------
					// �Q�[���̃��C������
					//--------------------

					GameMain();

					//--------------------

					//--------------------
					// �e��`�揈��
					//--------------------

					Draw();

					//--------------------

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

					PrintDebugProc("FPS : %d\n", g_nCountFPS);

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

					// Direct3D�ɂ��`��̏I��
					D3DDevice -> EndScene();

					// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
					D3DDevice -> Present( NULL, NULL, NULL, NULL );
				}
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod( 1 );	// �n�[�h�E�F�A�^�C�}�����̏�Ԃ֖߂��Ă���

	//--------------------
	// �e��I������
	//--------------------

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O�p�\���̏I������
	UnInitDebugProc();

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	// �J�����̏I������
	UnInitCamera();

	// DirectInput�̏I������
	ReleaseDInput();	// �L�[�{�[�h
	ReleaseDMouse();	// �}�E�X

	// �t�F�[�h�̏I������
	UninitFade();

	// �Q�[���Ŏg�p�������̂̏I������
	UnInit();

	// DirectX�I�u�W�F�N�g�̊J��
	ReleaseD3D();

	// COM�I������
	UnInitCOM();

	// �T�E���h�I������
	UninitSnd();

	//--------------------

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass( CLASS_NAME, wcex.hInstance );

	return( int )msg.wParam;
}

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	 S_OK	: �������ɐ���
//	 E_FAIL	: �������Ɏ��s
//
//============================================================================

HRESULT Init(void)
{
	HRESULT hr;

	// �J�����̏�����
	InitCamera();

	// ���C�g�̏�����
	InitLight();

	// �t�F�[�h�̏�����
	hr = InitFade();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O�\���̏�����
	hr = InitDebugProc();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	return S_OK;
}

//============================================================================
//
// �X�V���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void Updata(void)
{
	// �J�����̍X�V
	UpdataCamera();

	// �t�F�[�h�̍X�V
	UpdateFade();
}

//============================================================================
//
// �`�揈�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void Draw(void)
{
	// �J�����̕`��
	DrawCamera();

	// �t�F�[�h�̕`��
	DrawFade();

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O�\��
	DrawDebugProc();

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

}

//============================================================================
//
// �J������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UnInit(void)
{
	/* �Q�[���̏I������ */

	// �Q�[���J�ڌn�̏I������
	UninitTitle();
	UninitTutorial();
	UninitGameClear();
	UninitGameOver();

	// ���C���Q�[���̏I������

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

	// �X�R�A�̏I������
	UninitNumber();

	// �S�[���̏I������
	UnInitGoal();

	UninitReadGo();
}

//============================================================================
//
// ���Ԏ擾 ���� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_nTime
//
//============================================================================

int GetTime(void)
{
	return g_nTime;
}

/* EOF */