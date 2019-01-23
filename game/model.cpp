//===============================================================================================================
//
// model.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// インクルード
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

/* インプットに関するヘッダーファイル */
#include "input.h"
#include "mouse.h"

/* サウンドに関するヘッダーファイル */
#include "com.h"
#include "sound.h"
#include "snddata.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

// モデル
static MODEL g_Model;

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	モデルの作成
//	 S_OK	: モデルの読み込み成功
//	 E_FAIL	: モデルの読み込み失敗
//
//============================================================================

HRESULT InitModel( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// 地面の位置（向き、大きさ）初期化
	g_Model.pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// モデルの位置
	g_Model.rot = D3DXVECTOR3( 0.0f, D3DX_PI, 0.0f );	// モデルの向き
	g_Model.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// モデルの大きさ

	// 慣性
	g_Model.fAccelerationUp    = 0.01f;	// 上
	g_Model.fAccelerationDown  = 0.01f;	// 下
	g_Model.fAccelerationLeft  = 0.01f;	// 左
	g_Model.fAccelerationRight = 0.01f;	// 右

	// ジャンプ
	g_Model.fValueResist  = VALUE_RESIST;	// 移動時の抵抗を変数入れる
	g_Model.fValueJump    = VALUE_JUMP;		// ジャンプ量を変数入れる
	g_Model.fValueGravity = VALUE_GRAVITY;	// 重力（G）を変数入れる
	g_Model.bJump         = false;			// ジャンプフラグ

	// 向きたい角度
	g_Model.fDirection = D3DX_PI;		// 向きたい方向

	// ゴールフラグ
	g_Model.bGoal = false;

	// Xファイルの読み込み
	hr = D3DXLoadMeshFromX( MODEL_FILE,				// Xファイル名
							D3DXMESH_SYSTEMMEM,
							D3DDevice,				// DirectXデバイス
							NULL,
							&g_Model.pD3DXMatBuff,	// モデルのマテリアル情報のポインタ	
							NULL,
							&g_Model.nNumMat,		// マテリアル情報の総数
							&g_Model.pMeshModel );	// モデルのメッシュ情報のポインタ
	if( FAILED( hr ) )
	{
		return E_FAIL;	// Xファイルの読み込みに失敗
	}

	// テクスチャの読み込み
/*	hr = D3DXCreateTextureFromFile( D3DDevice, MODEL_TEXTURE_FILE, &g_Model.pD3DTextureModel );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
	}
*/
	// 影表示
	g_Model.fShadowSize = SHADOW_SIZE_MODEL;
	g_Model.nShadowNumber = SetShadow( g_Model.pos, g_Model.fShadowSize );

	return S_OK;
}

//============================================================================
//
// 更新処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UpdataModel( void )
{
	// マウスの情報取得
	MOUSE mouse = GetMouseState();

	// カメラの情報を取得
	CAMERA Camera = GetCamera();

	// 前回の座標保存
	g_Model.oldpos.x = g_Model.pos.x;
	g_Model.oldpos.z = g_Model.pos.z;

	//--------------------
	// 慣性の設定
	//--------------------

	// 奥
	g_Model.fAccelerationUp -= 0.005f;
	if( g_Model.fAccelerationUp < 0 )
	{
		g_Model.fAccelerationUp = 0.f;
	}

	// 手前
	g_Model.fAccelerationDown -= 0.005f;
	if( g_Model.fAccelerationDown < 0 )
	{
		g_Model.fAccelerationDown = 0.f;
	}

	// 左
	g_Model.fAccelerationLeft -= 0.005f;
	if( g_Model.fAccelerationLeft < 0 )
	{
		g_Model.fAccelerationLeft = 0.f;
	}

	// 右
	g_Model.fAccelerationRight -= 0.005f;
	if( g_Model.fAccelerationRight < 0 )
	{
		g_Model.fAccelerationRight = 0.f;
	}

	//--------------------
	// Inputの設定
	//--------------------

	// マウス入力で地面移動

	// マウスデバイスがある場合
	if( mouse.bUse == true )
	{
		if( PushClick( LEFT ) )
		{
			// 右
			if( 0 < mouse.x && mouse.x < SCREEN_WIDTH / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI / 2 * 3;
				}
			}

			// 左
			if( 0 > mouse.x && mouse.x > -SCREEN_WIDTH / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI / 2;
				}
			}

			// 奥
			if( 0 < mouse.y && mouse.y < SCREEN_HEIGHT / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = D3DX_PI;
				}
			}

			// 手前
			if( 0 > mouse.y && mouse.y > -SCREEN_HEIGHT / 2 )
			{
				if(g_Model.bGoal == false)
				{
					g_Model.fDirection = 0.f;
				}
			}
		}
	}
	
	// キー入力で地面移動

	// 奥に移動
	if( nowKey( DIK_UP ) || nowKey( DIK_W ) )			// ↑キー入力
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

	// 手前に移動
	if( nowKey( DIK_DOWN ) || nowKey( DIK_S ) )		// ↓キー入力
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

	// 左に移動
	if( nowKey( DIK_LEFT ) || nowKey( DIK_A ) )		// →キー入力
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

	// 右に移動
	if( nowKey( DIK_RIGHT ) || nowKey( DIK_D ) )		// ←キー入力
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
	// 内積・外積 計算
	//------------------------------

	float fFaceangle_X = -sinf( g_Model.fDirection );
	float fFaceangle_Z = -cosf( g_Model.fDirection );
	float fPlayer_X    = -sinf( g_Model.rot.y );
	float fPlayer_Z    = -cosf( g_Model.rot.y );

	// 内積計算
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

	// 外積計算
	//------------------------------

	float fCrossProduct = fFaceangle_Z * fPlayer_X - fFaceangle_X * fPlayer_Z;

	if( fCrossProduct >= 0 )
	{
		g_Model.rot += D3DXVECTOR3( 0.0f, -fAngle, 0.0f );	// モデルの向き
	}
	else if( fCrossProduct <= 0 )
	{
		g_Model.rot += D3DXVECTOR3( 0.0f, fAngle, 0.0f );	// モデルの向き
	}

	//------------------------------

	//------------------------------
	// 弾発射
	//------------------------------

	if( PushKey( DIK_SPACE ) || PushClick( RIGHT ) )		// キー入力
	{
		if(g_Model.bGoal == false)
		{
			// サウンド再生
			PlaySnd( SOUND_LABEL_SE001 );

			// 弾の作成
			SetBullet( g_Model.pos, g_Model.rot );
		}
	}

	//------------------------------

	//------------------------------
	// ジャンプ
	//------------------------------

	if(g_Model.bGoal == false)
	{
		// 地面の高さ
		float FieldPosY = 0.0f;

		// 重力
		g_Model.fValueGravity = VALUE_GRAVITY;

		if( PushKey( DIK_J ) )		// キー入力
		{
			// ジャンプ中でない時
			if( g_Model.bJump == false )
			{
				g_Model.bJump = true;
			}
		}

		// ジャンプした時
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
	
		// 着地
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
	// モデルの移動限界と当たり判定
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
				// サウンド再生
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
				// サウンド再生
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
				// サウンド再生
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
				// サウンド再生
				PlaySnd( SOUND_LABEL_SE005 );
			}
		}

		g_Model.fAccelerationRight = 0.f;
		g_Model.fAccelerationLeft  = 0.f;
		g_Model.fAccelerationUp    = 0.f;
		g_Model.fAccelerationDown  = 0.f;

		g_Model.pos.x = ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 200.f;
	}

	// ダンジョンの壁との当たり判定
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
					// サウンド再生
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

	// ゴールしたら手前を向く
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

	// その他の当たり判定

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

	// 影の更新
	ResetShadow( g_Model.nShadowNumber, g_Model.pos );

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ情報表示
	PrintDebugProc("Model POS : ( %.2f : %.2f : %.2f )\n", g_Model.pos.x, g_Model.pos.y, g_Model.pos.z);
	PrintDebugProc("Model ROT : ( %.2f : %.2f : %.2f )\n", g_Model.rot.x, g_Model.rot.y, g_Model.rot.z);

#endif //_DEBUG	//デバッグ用コード終了

	//--------------------
}

//============================================================================
//
// 描画処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void DrawModel( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	D3DXMATERIAL *pD3DXMat;

	//------------------------------
	// ワールドマトリックスの設定
	//------------------------------

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity( &g_Model.mtxWorld );

	// スケールを反映
	D3DXMatrixScaling( &mtxScl, g_Model.scl.x, g_Model.scl.y, g_Model.scl.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxScl );

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_Model.rot.y, g_Model.rot.x, g_Model.rot.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxRot );

	// 移動を反映
	D3DXMatrixTranslation( &mtxTrans, g_Model.pos.x, g_Model.pos.y, g_Model.pos.z );
	D3DXMatrixMultiply( &g_Model.mtxWorld, &g_Model.mtxWorld, &mtxTrans );

	// ワールドマトリックスの設定
	D3DDevice -> SetTransform( D3DTS_WORLD, &g_Model.mtxWorld );

	//------------------------------

	//--------------------
	// モデル描画処理
	//--------------------

	pD3DXMat = ( D3DXMATERIAL* ) g_Model.pD3DXMatBuff -> GetBufferPointer();

	for( int nCntMat = 0; nCntMat < ( int )g_Model.nNumMat; nCntMat++ )
	{
		// マテリアルの設定
		D3DDevice -> SetMaterial( &pD3DXMat[nCntMat].MatD3D );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_Model.pD3DTextureModel );

		g_Model.pMeshModel -> DrawSubset( nCntMat );
	}
	
	//--------------------
}

//============================================================================
//
// 開放処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UnInitModel(void)
{
	// モデルのテクスチャ終了処理
	if( g_Model.pD3DTextureModel != NULL )
	{
		g_Model.pD3DTextureModel -> Release();
		g_Model.pD3DTextureModel = NULL;
	}
	// モデルのメッシュ情報のポインタの開放
	if( g_Model.pMeshModel != NULL )
	{
		g_Model.pMeshModel -> Release();
		g_Model.pMeshModel = NULL;
	}
	// モデルのマテリアル情報のポインタの開放
	if( g_Model.pD3DXMatBuff != NULL )
	{
		g_Model.pD3DXMatBuff -> Release();
		g_Model.pD3DXMatBuff = NULL;
	}
}

//============================================================================
//
// モデルのポジション取得 関数
// 引数：
//　なし
// 戻り値：
//　g_Model
//
//============================================================================

MODEL* GetModel(void)
{
	return &g_Model;
}

/* EOF */