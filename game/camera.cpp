//===============================================================================================================
//
// camera.cpp
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
#include "input.h"
#include "camera.h"
#include "model.h"
#include "meshfield.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

static CAMERA g_camera;

static float g_fPosY;

static float Radian = D3DXToRadian( 90 );

//============================================================================
//
// �J�����̏����� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void InitCamera( void )
{
	// ���f���̏��擾
	MODEL *Model = GetModel();

	g_fPosY = 400.0f;

	// �J�����̎��_
	g_camera.posP = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + g_fPosY, Model -> pos.z - 300.0f );

	// �J�����̒����_
	g_camera.posR = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + 50.0f, Model -> pos.z );

	// �J�����̏���x�N�g��
	g_camera.posU = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	// �J�����̉�]
	g_camera.rot = D3DXVECTOR3( 0.0f, D3DXToRadian( 90 ), 0.0f );

	/* �J�����̎��_�ƒ����_�̊Ԃ̋����Z�o */
	g_camera.fLengthIntervalCamera = g_camera.posP.z - g_camera.posR.z;	// �J�����̎��_��Z - �J�����̒����_��Z

}

//============================================================================
//
// �J�����̍X�V �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UpdataCamera( void )
{
	// ���f���̏��擾
	MODEL *Model = GetModel();

	// �S�[��������J���������f���ɋ߂Â���
	if( Model -> bGoal == true )
	{
		g_fPosY -= 1.f;
		if(g_fPosY < 100.f)
		{
			g_fPosY = 100.f;
		}
	}
	else
	{
		g_fPosY = 400.f;
	}

	// �J�����̎��_
	g_camera.posP = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + g_fPosY, Model -> pos.z - 300.0f );
	
	if( g_camera.posP.z < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) )
	{
		g_camera.posP.z = ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) );
	}

	// �J�����̒����_
	g_camera.posR = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + 50.0f, Model -> pos.z );

/*
#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	//--------------------
	// Input�̐ݒ�
	//--------------------

	// �L�[���͂ŃJ����
	if( nowKey( DIK_W ) )		// W�L�[����
	{
		g_camera.posP.x += cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posP.z += sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.x += cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.z += sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_D ) )		// D�L�[����
	{
		g_camera.posP.x -= cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posP.z -= sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.x -= cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.z -= sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_S ) )		// S�L�[����
	{
		g_camera.posP.x -= cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posP.z -= sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.x -= cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.z -= sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_A ) )		// A�L�[����
	{
		g_camera.posP.x += cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posP.z += sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.x += cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.z += sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
	}

	// C �L�[����
	if( nowKey( DIK_C ) )
	{
		// �E��]
		g_camera.rot.y += D3DXToRadian( ROTATION );
		RotationCamera();
	}
	// Z �L�[����
	if( nowKey( DIK_Z ) )
	{
		// �E��]
		g_camera.rot.y -= D3DXToRadian( ROTATION );
		RotationCamera();
	}
	float tempRad;
	// Q �L�[����
	if( nowKey( DIK_Q ) )
	{
		tempRad = g_camera.rot.y - D3DXToRadian( 180 ) + 0.03f;
		g_camera.rot.y += 0.03f;
		g_camera.posR.x = g_camera.posP.x + cos( tempRad ) * g_camera.fLengthIntervalCamera;
		g_camera.posR.z = g_camera.posP.z + sin( tempRad ) * g_camera.fLengthIntervalCamera;
	}
	// E �L�[����
	if( nowKey( DIK_E ) )
	{
		tempRad = g_camera.rot.y - D3DXToRadian( 180 ) - 0.03f;
		g_camera.rot.y -= 0.03f;
		g_camera.posR.x = g_camera.posP.x + cos( tempRad ) * g_camera.fLengthIntervalCamera;
		g_camera.posR.z = g_camera.posP.z + sin( tempRad ) * g_camera.fLengthIntervalCamera;
	}

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	//--------------------
*/
}

//============================================================================
//
// �J�����̕`�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void DrawCamera( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	//-----------------------------------
	// �r���[�}�g���b�N�X
	//-----------------------------------

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_camera.mtxView );

	D3DXMatrixLookAtLH( &g_camera.mtxView, &g_camera.posP, &g_camera.posR, &g_camera.posU );

	// �r���[�}�g���b�N�X�̐ݒ�
	D3DDevice -> SetTransform( D3DTS_VIEW, &g_camera.mtxView );

	//-----------------------------------

	//-----------------------------------
	// �v���W�F�N�g�}�g���b�N�X
	//-----------------------------------

	// �v���W�F�N�g�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_camera.mtxProjection );

	D3DXMatrixPerspectiveFovLH( &g_camera.mtxProjection,
							    D3DX_PI / 4,							// ����p�i�S�T�x�j
							    ( float )SCREEN_WIDTH / SCREEN_HEIGHT,	// �A�X�y�N�g��@�i ��ʂ̕� / ���� �j
							    10.0f,									// NecrZ
							    3000.0f);								// FarZ

	// �v���W�F�N�g�}�g���b�N�X�̐ݒ�
	D3DDevice -> SetTransform( D3DTS_PROJECTION, &g_camera.mtxProjection );

	//-----------------------------------
}

//============================================================================
//
// �J�����̏I�� �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void UnInitCamera( void )
{
	;
}

//============================================================================
//
// �J�����̉�]���� �֐�
// �����F
//�@float Rota�@�c�@��]��
// �߂�l�F
//�@�Ȃ�
//
//============================================================================

void RotationCamera( void )
{
	// ���f���̏��擾
	MODEL *Model = GetModel();

	float X;	// �J�����̌����iX���W�j�p�ϐ�
	float Z;	// �J�����̌����iZ���W�j�p�ϐ�

	// cos(��]��) * �J�����̎��_�ƒ����_�̊Ԃ̋������Z�o + �����_��X
	X = cos( Model -> rot.y + D3DXToRadian( 90 ) ) * g_camera.fLengthIntervalCamera + g_camera.posR.x;

	// sin(��]��) * �J�����̎��_�ƒ����_�̊Ԃ̋������Z�o + �����_��Z
	Z = sin( Model -> rot.y + D3DXToRadian( -90 ) ) * g_camera.fLengthIntervalCamera + g_camera.posR.z;

	g_camera.posP = D3DXVECTOR3( X, g_camera.posP.y, Z );
}

//============================================================================
//
// �J�����̏��擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_camera
//
//============================================================================

CAMERA GetCamera( void )
{
	return g_camera;
}

/* EOF */