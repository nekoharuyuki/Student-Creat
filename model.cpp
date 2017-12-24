//===============================================================================================================
//
// model.cpp
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
#include "model.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "camera.h"
#include "shadow.h"
#include "dungeon.h"

/* �C���v�b�g�Ɋւ���w�b�_�[�t�@�C�� */
#include "input.h"
#include "mouse.h"

/* �T�E���h�Ɋւ���w�b�_�[�t�@�C�� */
#include "com.h"
#include "sound.h"
#include "snddata.h"

//============================================================================
//
// �O���[�o���ϐ�
//
//============================================================================

// ���f��
static MODEL g_Model;

//============================================================================
//
// ������ �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//	���f���̍쐬
//	 S_OK	: ���f���̓ǂݍ��ݐ���
//	 E_FAIL	: ���f���̓ǂݍ��ݎ��s
//
//============================================================================

HRESULT InitModel( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// �n�ʂ̈ʒu�i�����A�傫���j������
	g_Model.pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// ���f���̈ʒu
	g_Model.rot = D3DXVECTOR3( 0.0f, D3DX_PI, 0.0f );	// ���f���̌���
	g_Model.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// ���f���̑傫��

	// ����
	g_Model.fAccelerationUp    = 0.01f;	// ��
	g_Model.fAccelerationDown  = 0.01f;	// ��
	g_Model.fAccelerationLeft  = 0.01f;	// ��
	g_Model.fAccelerationRight = 0.01f;	// �E

	// �W�����v
	g_Model.fValueResist  = VALUE_RESIST;	// �ړ����̒�R��ϐ������
	g_Model.fValueJump    = VALUE_JUMP;		// �W�����v�ʂ�ϐ������
	g_Model.fValueGravity = VALUE_GRAVITY;	// �d�́iG�j��ϐ������
	g_Model.bJump         = false;			// �W�����v�t���O

	// ���������p�x
	g_Model.fDirection = D3DX_PI;		// ������������

	// �S�[���t���O
	g_Model.bGoal = false;

	// X�t�@�C���̓ǂݍ���
	hr = D3DXLoadMeshFromX( MODEL_FILE,				// X�t�@�C����
							D3DXMESH_SYSTEMMEM,
							D3DDevice,				// DirectX�f�o�C�X
							NULL,
							&g_Model.pD3DXMatBuff,	// ���f���̃}�e���A�����̃|�C���^	
							NULL,
							&g_Model.nNumMat,		// �}�e���A�����̑���
							&g_Model.pMeshModel );	// ���f���̃��b�V�����̃|�C���^
	if( FAILED( hr ) )
	{
		return E_FAIL;	// X�t�@�C���̓ǂݍ��݂Ɏ��s
	}

	// �e�N�X�`���̓ǂݍ���
/*	hr = D3DXCreateTextureFromFile( D3DDevice, MODEL_TEXTURE_FILE, &g_Model.pD3DTextureModel );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// �e�N�X�`���̓ǂݍ��݂Ɏ��s
	}
*/
	// �e�\��
	g_Model.fShadowSize = SHADOW_SIZE_MODEL;
	g_Model.nShadowNumber = SetShadow( g_Model.pos, g_Model.fShadowSize );

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

void UpdataModel( void )
{
	// �}�E�X�̏��擾
	MOUSE mouse = GetMouseState();

	// �J�����̏����擾
	CAMERA Camera = GetCamera();

	// �O��̍��W�ۑ�
	g_Model.oldpos.x = g_Model.pos.x;
	g_Model.oldpos.z = g_Model.pos.z;

	//--------------------
	// �����̐ݒ�
	//--------------------

	// ��
	g_Model.fAccelerationUp -= 0.005f;
	if( g_Model.fAccelerationUp < 0 )
	{
		g_Model.fAccelerationUp = 0.f;
	}

	// ��O
	g_Model.fAccelerationDown -= 0.005f;
	if( g_Model.fAccelerationDown < 0 )
	{
		g_Model.fAccelerationDown = 0.f;
	}

	// ��
	g_Model.fAccelerationLeft -= 0.005f;
	if( g_Model.fAccelerationLeft < 0 )
	{
		g_Model.fAccelerationLeft = 0.f;
	}

	// �E
	g_Model.fAccelerationRight -= 0.005f;
	if( g_Model.fAccelerationRight < 0 )
	{
		g_Model.fAccelerationRight = 0.f;
	}

	//--------------------
	// Input�̐ݒ�
	//--------------------

	// �}�E�X���͂Œn�ʈړ�

	// �}�E�X�f�o�C�X������ꍇ
	if( mouse.bUse == true )
	{
		if( PushClick( LEFT ) )
		{
			// �E
			if( 0 < mouse.x && mouse.x < SCREEN_WIDTH / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI / 2 * 3;
				}
			}

			// ��
			if( 0 > mouse.x && mouse.x > -SCREEN_WIDTH / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI / 2;
				}
			}

			// ��
			if( 0 < mouse.y && mouse.y < SCREEN_HEIGHT / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI;
				}
			}

			// ��O
			if( 0 > mouse.y && mouse.y > -SCREEN_HEIGHT / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = 0.f;
				}
			}
		}
	}
	
	// �L�[���͂Œn�ʈړ�

	// ���Ɉړ�
	if( nowKey( DIK_UP ) || nowKey( DIK_W ) )			// ���L�[����
	{
		if(g_Model.bGoal == false)
		{
			g_Model.fDirection = D3DX_PI;

			g_Model.fAccelerationUp += 0.08f;
			if( g_Model.fAccelerationUp >= 1 )
			{
				g_Model.fAccelerationUp = 1.0f;
			}
		}
	}

	g_Model.pos.x += cos( Camera.rot.y ) * g_Model.fAccelerationUp * MODEL_MOVE_SPEED;
	g_Model.pos.z += sin( Camera.rot.y ) * g_Model.fAccelerationUp * MODEL_MOVE_SPEED;

	// ��O�Ɉړ�
	if( nowKey( DIK_DOWN ) || nowKey( DIK_S ) )		// ���L�[����
	{
		if(g_Model.bGoal == false)
		{
			g_Model.fDirection = 0.f;
			
			g_Model.fAccelerationDown += 0.08f;
			if( g_Model.fAccelerationDown >= 1 )
			{
				g_Model.fAccelerationDown = 1.0f;
			}
		}
	}

	g_Model.pos.x -= cos( Camera.rot.y ) * g_Model.fAccelerationDown * MODEL_MOVE_SPEED;
	g_Model.pos.z -= sin( Camera.rot.y ) * g_Model.fAccelerationDown * MODEL_MOVE_SPEED;

	// ���Ɉړ�
	if( nowKey( DIK_LEFT ) || nowKey( DIK_A ) )		// ���L�[����
	{
		if(g_Model.bGoal == false)
		{
			g_Model.fDirection = D3DX_PI / 2;

			g_Model.fAccelerationLeft += 0.08f;
			
			if( g_Model.fAccelerationLeft >= 1 )
			{
				g_Model.fAccelerationLeft = 1.0f;
			}
		}
	}

	g_Model.pos.x += cos( Camera.rot.y + D3DXToRadian( 90 ) ) * g_Model.fAccelerationLeft * MODEL_MOVE_SPEED;
	g_Model.pos.z += sin( Camera.rot.y + D3DXToRadian( 90 ) ) * g_Model.fAccelerationLeft * MODEL_MOVE_SPEED;

	// �E�Ɉړ�
	if( nowKey( DIK_RIGHT ) || nowKey( DIK_D ) )		// ���L�[����
	{
		if(g_Model.bGoal == false)
		{
			g_Model.fDirection = D3DX_PI / 2 * 3;

			g_Model.fAccelerationRight += 0.08f;		
			if( g_Model.fAccelerationRight >= 1 )
			{
				g_Model.fAccelerationRight = 1.0f;
			}
		}
	}

	g_Model.pos.x -= cos( Camera.rot.y + D3DXToRadian( 90 ) ) * g_Model.fAccelerationRight * MODEL_MOVE_SPEED;
	g_Model.pos.z -= sin( Camera.rot.y + D3DXToRadian( 90 ) ) * g_Model.fAccelerationRight * MODEL_MOVE_SPEED;

	

	//------------------------------
	// ���ρE�O�� �v�Z
	//------------------------------

	float fFaceangle_X = -sinf( g_Model.fDirection );
	float fFaceangle_Z = -cosf( g_Model.fDirection );
	float fPlayer_X    = -sinf( g_Model.rot.y );
	float fPlayer_Z    = -cosf( g_Model.rot.y );

	// ���όv�Z
	//------------------------------

	float fDotProduct = fFaceangle_X * fPlayer_X + fFaceangle_Z * fPlayer_Z;

	if( fDotProduct > 1 )
	{
		fDotProduct = 1;
	}
	else if( fDotProduct < -1 )
	{
		fDotProduct = -1;
	}

	float fAngle = acos( fDotProduct );
	fAngle /= 20;

	// �O�όv�Z
	//------------------------------

	float fCrossProduct = fFaceangle_Z * fPlayer_X - fFaceangle_X * fPlayer_Z;

	if( fCrossProduct >= 0 )
	{
		g_Model.rot += D3DXVECTOR3( 0.0f, -fAngle, 0.0f );	// ���f���̌���
	}
	else if( fCrossProduct <= 0 )
	{
		g_Model.rot += D3DXVECTOR3( 0.0f, fAngle, 0.0f );	// ���f���̌���
	}

	//------------------------------

	//------------------------------
	// �e����
	//------------------------------

	if( PushKey( DIK_SPACE ) || PushClick( RIGHT ) )		// �L�[����
	{
		if(g_Model.bGoal == false)
		{
			// �T�E���h�Đ�
			PlaySnd( SOUND_LABEL_SE001 );

			// �e�̍쐬
			SetBullet( g_Model.pos, g_Model.rot );
		}
	}

	//------------------------------

	//------------------------------
	// �W�����v
	//------------------------------

	if(g_Model.bGoal == false)
	{
		// �n�ʂ̍���
		float FieldPosY = 0.0f;

		// �d��
		g_Model.fValueGravity = VALUE_GRAVITY;

		if( PushKey( DIK_J ) )		// �L�[����
		{
			// �W�����v���łȂ���
			if( g_Model.bJump == false )
			{
				g_Model.bJump = true;
			}
		}

		// �W�����v������
		if( g_Model.bJump == true )
		{		
			g_Model.fValueResist += 0.1f;
			g_Model.pos.y += g_Model.fValueJump;
			g_Model.fValueJump -= g_Model.fValueResist;
		}
		else
		{
			g_Model.pos.y = FieldPosY;
		}
	
		// ���n
		if( g_Model.pos.y < FieldPosY )
		{
			g_Model.bJump = false;
			g_Model.fValueJump = VALUE_JUMP;
			g_Model.fValueResist = 0;
			g_Model.pos.y = FieldPosY;
		}
	}


	//------------------------------

	//------------------------------
	// ���f���̈ړ����E�Ɠ����蔻��
	//------------------------------

	if( g_Model.pos.z < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) + MODEL_SIZE )
	{
		if(g_Model.bGoal == false)
		{
			if( PushKey( DIK_UP )    ||
				PushKey( DIK_DOWN )  ||
				PushKey( DIK_LEFT )  ||
				PushKey( DIK_RIGHT )
				)
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE005 );
			}
		}

		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;

		g_Model.pos.z = ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) + MODEL_SIZE;
	}
	if( g_Model.pos.z > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 200.f )
	{
		if(g_Model.bGoal == false)
		{
			if( PushKey( DIK_UP )   ||
				PushKey( DIK_DOWN ) ||
				PushKey( DIK_LEFT ) ||
				PushKey( DIK_RIGHT )
				)
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE005 );
			}
		}

		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;

		g_Model.pos.z = ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 200.f;
	}
	if( g_Model.pos.x < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) + MODEL_SIZE )
	{
		if(g_Model.bGoal == false)
		{
			if( PushKey( DIK_UP )   ||
				PushKey( DIK_DOWN ) ||
				PushKey( DIK_LEFT ) ||
				PushKey( DIK_RIGHT )
				)
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE005 );
			}
		}

		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;

		g_Model.pos.x = ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) + MODEL_SIZE;
	}
	if( g_Model.pos.x > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 200.f )
	{
		if(g_Model.bGoal == false)
		{
			if( PushKey( DIK_UP )   ||
				PushKey( DIK_DOWN ) ||
				PushKey( DIK_LEFT ) ||
				PushKey( DIK_RIGHT )
				)
			{
				// �T�E���h�Đ�
				PlaySnd( SOUND_LABEL_SE005 );
			}
		}

		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;

		g_Model.pos.x = ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 200.f;
	}

	// �_���W�����̕ǂƂ̓����蔻��
	float tempPosX, tempPosZ;
	float tempRad = 0;
	for( int i = 0; i < 10; i++ )
	{
		tempPosX = g_Model.pos.x + cosf( tempRad ) * 60;
		tempPosZ = g_Model.pos.z + sinf( tempRad ) * 60;
		tempRad += ( D3DX_PI * 2 ) / 10;

		int posX= ( int )( tempPosX + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );
		int posZ= ( int )( tempPosZ + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );

		int Dungeon = GetDungeon( ( int )( posX ), ( int )( posZ ) );

		if( Dungeon == 1 )
		{
			if(g_Model.bGoal == false)
			{
				if( PushKey( DIK_UP )   ||
					PushKey( DIK_DOWN ) ||
					PushKey( DIK_LEFT ) ||
					PushKey( DIK_RIGHT )
					)
				{
					// �T�E���h�Đ�
					PlaySnd( SOUND_LABEL_SE005 );
				}
			}

			g_Model.pos.x = g_Model.oldpos.x;
			g_Model.pos.z = g_Model.oldpos.z;
			g_Model.fAccelerationRight = 0.f;
			g_Model.fAccelerationLeft  = 0.f;
			g_Model.fAccelerationUp    = 0.f;
			g_Model.fAccelerationDown  = 0.f;
			break;
		}
		if( Dungeon == 2 )
		{
			if(g_Model.bGoal == false)
			{
				g_Model.bGoal = true;
			}
			break;
		}
	}

	// �S�[���������O������
	if( g_Model.bGoal              == true && 
	    g_Model.fAccelerationUp    == 0.f  &&
	    g_Model.fAccelerationDown  == 0.f  &&
	    g_Model.fAccelerationLeft  == 0.f  &&
	    g_Model.fAccelerationRight == 0.f )
	{
		g_Model.fDirection = 0.f;
	}

	//------------------------------

	/*

	// ���̑��̓����蔻��

	int posX= (int)( g_Model.pos.x + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );
	int posZ= (int)( g_Model.pos.z + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );

	int Dungeon = GetDungeon( ( int )( posX ), ( int )( posZ ) );

	if( Dungeon == 1 )
	{
		g_Model.pos.x = g_Model.oldpos.x;
		g_Model.pos.z = g_Model.oldpos.z;
		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;
	}
	*/
	/*
	int tempX = (g_Model.pos.x - sinf( g_Model.rot.y ) * 60);
	int tempZ = (g_Model.pos.z - cosf( g_Model.rot.y ) * 60);
	int posX= (int)(tempX  + ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) / ( TEXTURE_NUM * TEXTURE_SIZE );
	int posZ= (int)(tempZ  + ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) / ( TEXTURE_NUM * TEXTURE_SIZE );

	int Dungeon = GetDungeon( ( int )( posX ), ( int )( posZ ) );

	if( Dungeon == 1 )
	{
		g_Model.pos.x = g_Model.oldpos.x;
		g_Model.pos.z = g_Model.oldpos.z;
		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;
	}
	*/

	// �e�̍X�V
	ResetShadow( g_Model.nShadowNumber, g_Model.pos );

#ifdef _DEBUG	//�f�o�b�O�p�R�[�h�J�n

	// �f�o�b�O���\��
	PrintDebugProc("Model POS : ( %.2f : %.2f : %.2f )\n", g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	PrintDebugProc("Model ROT : ( %.2f : %.2f : %.2f )\n", g_Model.rot.x, g_Model.rot.y, g_Model.rot.z);

#endif //_DEBUG	//�f�o�b�O�p�R�[�h�I��

	//--------------------
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

void DrawModel( void )
{
	// DirectX�f�o�C�X�̒l���擾
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	D3DXMATERIAL *pD3DXMat;

	//------------------------------
	// ���[���h�}�g���b�N�X�̐ݒ�
	//------------------------------

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_Model.mtxWorld );

	// �X�P�[���𔽉f
	D3DXMatrixScaling( &mtxScl, g_Model.scl.x, g_Model.scl.y, g_Model.scl.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxScl );

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxRot );

	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxTrans );

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DDevice -> SetTransform( D3DTS_WORLD, &g_Model.mtxWorld );

	//------------------------------

	//--------------------
	// ���f���`�揈��
	//--------------------

	pD3DXMat = ( D3DXMATERIAL* ) g_Model.pD3DXMatBuff -> GetBufferPointer();

	for( int nCntMat = 0; nCntMat < ( int )g_Model.nNumMat; nCntMat++ )
	{
		// �}�e���A���̐ݒ�
		D3DDevice -> SetMaterial( &pD3DXMat[nCntMat].MatD3D );

		// �e�N�X�`���̐ݒ�
		D3DDevice -> SetTexture( 0, g_Model.pD3DTextureModel );

		g_Model.pMeshModel -> DrawSubset( nCntMat );
	}
	
	//--------------------
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

void UnInitModel(void)
{
	// ���f���̃e�N�X�`���I������
	if( g_Model.pD3DTextureModel != NULL )
	{
		g_Model.pD3DTextureModel -> Release();
		g_Model.pD3DTextureModel = NULL;
	}
	// ���f���̃��b�V�����̃|�C���^�̊J��
	if( g_Model.pMeshModel != NULL )
	{
		g_Model.pMeshModel -> Release();
		g_Model.pMeshModel = NULL;
	}
	// ���f���̃}�e���A�����̃|�C���^�̊J��
	if( g_Model.pD3DXMatBuff != NULL )
	{
		g_Model.pD3DXMatBuff -> Release();
		g_Model.pD3DXMatBuff = NULL;
	}
}

//============================================================================
//
// ���f���̃|�W�V�����擾 �֐�
// �����F
//�@�Ȃ�
// �߂�l�F
//�@g_Model
//
//============================================================================

MODEL* GetModel(void)
{
	return &g_Model;
}

/* EOF */