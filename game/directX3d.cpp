//===============================================================================================================
//
// DirectX3D.cpp
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

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// �C���^�[�t�F�C�X
static LPDIRECT3D9           g_pD3D       = NULL;	// Direct3D�I�u�W�F�N�g
static LPDIRECT3DDEVICE9     g_pD3DDevice = NULL;	// Direct�I�u�W�F�N�g�i�`��ɕK�v�j

//============================================================================
//
// DirectX�̏����� �֐�
// �����F
//  void
// �߂�l�F
//�@DirectX �������̏�������
//		S_OK	�c�@����
//		E_FAIL	�c�@���s
//
//============================================================================

HRESULT InitDX( HWND hWnd )
{
	HRESULT hr;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );	// Direct3D9�C���^�[�t�F�C�X�̎擾
	
	if( NULL == g_pD3D )
	{
		return E_FAIL;	// �擾�Ɏ��s
	}
	
	// ���݂̃f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE d3ddm;
	hr = g_pD3D -> GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
	if( FAILED( hr ) )
	{
		return E_FAIL;	// ���s�����ꍇ
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );									// ���[�N���[���N���A
	d3dpp.BackBufferCount                = 1;								// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth                = SCREEN_WIDTH;					// �Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight               = SCREEN_HEIGHT;					// �Q�[����ʃT�C�Y�i�����j
	d3dpp.SwapEffect                     = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v
	d3dpp.Windowed                       = TRUE;							// �E�C���h�E���[�h
	d3dpp.EnableAutoDepthStencil         = TRUE;							// �f�v�X�o�b�t�@�iZ�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat         = D3DFMT_D16;						// 16bit���f�v�X�o�b�t�@�iZ�o�b�t�@�j�Ƃ��Ďg�p

	if( TRUE == d3dpp.Windowed )
	{	// �E�C���h�E���[�h
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = 0;								// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// �C���^�[�o��
	}
	else
	{	// �t���X�N���[�����[�h
		d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// �o�b�N�o�b�t�@
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// �C���^�[�o��
	}

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	struct{
		D3DDEVTYPE DevType;
		DWORD BehaviorFlags;
	} DevTypes [] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// �`��ƒ��_�������n�[�h�E�F�A�v�Z
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// �`����n�[�h�E�F�A�C���_������CPU
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// �`���CPU�C���_�������n�[�h�E�F�A
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// �`��ƒ��_������CPU�v�Z
	};
	
	D3DDEVTYPE DevType;
	DWORD BehaviorFlags;
	BOOL bCreateDevice;
	bCreateDevice = FALSE;
	
	for( int nLoop = 0; nLoop < _countof( DevTypes ); nLoop++ )
	{
		DevType       = DevTypes[ nLoop ].DevType;
		BehaviorFlags = DevTypes[ nLoop ].BehaviorFlags;
		hr = g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT,	// �f�B�X�v���C�A�_�v�^
									 DevType,				// �f�B�X�v���C�^�C�v
									 hWnd,					// �E�C���h�E�n���h��
									 BehaviorFlags,			// �f�o�C�X�쐬����̑g�ݍ��킹
									 &d3dpp,				// �v���[���e�[�V�����p�����[�^
									 &g_pD3DDevice );		// �f�o�C�X�C���^�[�t�F�[�X�|�C���^

		if( SUCCEEDED( hr ) )
		{	// �f�o�C�X�쐬���������Ȃ�΁A���[�v�𔲂���
			bCreateDevice = TRUE;
			break;
		}
	}
	
	if( FALSE == bCreateDevice )
	{	// �f�o�C�X�쐬���s
		return( E_FAIL );
	}

	// �����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice -> SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// �J�����O���s��
	g_pD3DDevice -> SetRenderState( D3DRS_ZENABLE, TRUE );						// Z�o�b�t�@���g�p
	g_pD3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			// ���u�����h���g�p
	g_pD3DDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// ���\�[�X�J���[�̎w��
	g_pD3DDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// ���f�B�X�e�B�l�[�V�����J���[�̎w��
	
	// �T���v���[�X�e�[�g�p�����[�^�̐ݒ�
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );	// �e�N�X�`��U�l�̌J�Ԑݒ�
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );	// �e�N�X�`��V�l�̌J�Ԑݒ�
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// �e�N�X�`���g�厞�̕�Ԑݒ�
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// �e�N�X�`���g�����̕�Ԑݒ�

	return S_OK;
}

//============================================================================
//
// DirectX�f�o�C�X�ƃI�u�W�F�N�g�̊J�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void ReleaseD3D( void )
{
	if( NULL != g_pD3DDevice )
	{	// �f�o�C�X�̊J��
		g_pD3DDevice -> Release();
		g_pD3DDevice = NULL;
	}
	
	if( NULL != g_pD3D )
	{	// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D -> Release();
		g_pD3D = NULL;
	}
}

//============================================================================
//
// DirectX�f�o�C�X�̃Q�b�^�[ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_pD3DDevice
//
//============================================================================

LPDIRECT3DDEVICE9 GetD3DDevice( void )
{
	return g_pD3DDevice;
}

/* EOF */