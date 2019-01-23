//===============================================================================================================
//
// camera.cpp
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
#include "input.h"
#include "camera.h"
#include "model.h"
#include "meshfield.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

static CAMERA g_camera;

static float g_fPosY;

static float Radian = D3DXToRadian( 90 );

//============================================================================
//
// カメラの初期化 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void InitCamera( void )
{
	// モデルの情報取得
	MODEL *Model = GetModel();

	g_fPosY = 400.0f;

	// カメラの視点
	g_camera.posP = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + g_fPosY, Model -> pos.z - 300.0f );

	// カメラの注視点
	g_camera.posR = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + 50.0f, Model -> pos.z );

	// カメラの上向ベクトル
	g_camera.posU = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	// カメラの回転
	g_camera.rot = D3DXVECTOR3( 0.0f, D3DXToRadian( 90 ), 0.0f );

	/* カメラの視点と注視点の間の距離算出 */
	g_camera.fLengthIntervalCamera = g_camera.posP.z - g_camera.posR.z;	// カメラの視点のZ - カメラの注視点のZ

}

//============================================================================
//
// カメラの更新 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UpdataCamera( void )
{
	// モデルの情報取得
	MODEL *Model = GetModel();

	// ゴールしたらカメラをモデルに近づける
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

	// カメラの視点
	g_camera.posP = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + g_fPosY, Model -> pos.z - 300.0f );
	
	if( g_camera.posP.z < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) ) )
	{
		g_camera.posP.z = ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) );
	}

	// カメラの注視点
	g_camera.posR = D3DXVECTOR3( Model -> pos.x, Model -> pos.y + 50.0f, Model -> pos.z );

/*
#ifdef _DEBUG	//デバッグ用コード開始

	//--------------------
	// Inputの設定
	//--------------------

	// キー入力でカメラ
	if( nowKey( DIK_W ) )		// Wキー入力
	{
		g_camera.posP.x += cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posP.z += sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.x += cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.z += sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_D ) )		// Dキー入力
	{
		g_camera.posP.x -= cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posP.z -= sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.x -= cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.z -= sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_S ) )		// Sキー入力
	{
		g_camera.posP.x -= cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posP.z -= sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.x -= cos( g_camera.rot.y ) * MODEL_MOVE_SPEED;
		g_camera.posR.z -= sin( g_camera.rot.y ) * MODEL_MOVE_SPEED;
	}
	if( nowKey( DIK_A ) )		// Aキー入力
	{
		g_camera.posP.x += cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posP.z += sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.x += cos( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
		g_camera.posR.z += sin( g_camera.rot.y + D3DXToRadian( 90 ) ) * MODEL_MOVE_SPEED;
	}

	// C キー入力
	if( nowKey( DIK_C ) )
	{
		// 右回転
		g_camera.rot.y += D3DXToRadian( ROTATION );
		RotationCamera();
	}
	// Z キー入力
	if( nowKey( DIK_Z ) )
	{
		// 右回転
		g_camera.rot.y -= D3DXToRadian( ROTATION );
		RotationCamera();
	}
	float tempRad;
	// Q キー入力
	if( nowKey( DIK_Q ) )
	{
		tempRad = g_camera.rot.y - D3DXToRadian( 180 ) + 0.03f;
		g_camera.rot.y += 0.03f;
		g_camera.posR.x = g_camera.posP.x + cos( tempRad ) * g_camera.fLengthIntervalCamera;
		g_camera.posR.z = g_camera.posP.z + sin( tempRad ) * g_camera.fLengthIntervalCamera;
	}
	// E キー入力
	if( nowKey( DIK_E ) )
	{
		tempRad = g_camera.rot.y - D3DXToRadian( 180 ) - 0.03f;
		g_camera.rot.y -= 0.03f;
		g_camera.posR.x = g_camera.posP.x + cos( tempRad ) * g_camera.fLengthIntervalCamera;
		g_camera.posR.z = g_camera.posP.z + sin( tempRad ) * g_camera.fLengthIntervalCamera;
	}

#endif //_DEBUG	//デバッグ用コード終了

	//--------------------
*/
}

//============================================================================
//
// カメラの描画 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void DrawCamera( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	//-----------------------------------
	// ビューマトリックス
	//-----------------------------------

	// ビューマトリックスの初期化
	D3DXMatrixIdentity( &g_camera.mtxView );

	D3DXMatrixLookAtLH( &g_camera.mtxView, &g_camera.posP, &g_camera.posR, &g_camera.posU );

	// ビューマトリックスの設定
	D3DDevice -> SetTransform( D3DTS_VIEW, &g_camera.mtxView );

	//-----------------------------------

	//-----------------------------------
	// プロジェクトマトリックス
	//-----------------------------------

	// プロジェクトマトリックスの初期化
	D3DXMatrixIdentity( &g_camera.mtxProjection );

	D3DXMatrixPerspectiveFovLH( &g_camera.mtxProjection,
							    D3DX_PI / 4,							// 視野角（４５度）
							    ( float )SCREEN_WIDTH / SCREEN_HEIGHT,	// アスペクト比　（ 画面の幅 / 高さ ）
							    10.0f,									// NecrZ
							    3000.0f);								// FarZ

	// プロジェクトマトリックスの設定
	D3DDevice -> SetTransform( D3DTS_PROJECTION, &g_camera.mtxProjection );

	//-----------------------------------
}

//============================================================================
//
// カメラの終了 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UnInitCamera( void )
{
	;
}

//============================================================================
//
// カメラの回転処理 関数
// 引数：
//　float Rota　…　回転量
// 戻り値：
//　なし
//
//============================================================================

void RotationCamera( void )
{
	// モデルの情報取得
	MODEL *Model = GetModel();

	float X;	// カメラの向き（X座標）用変数
	float Z;	// カメラの向き（Z座標）用変数

	// cos(回転量) * カメラの視点と注視点の間の距離を算出 + 注視点のX
	X = cos( Model -> rot.y + D3DXToRadian( 90 ) ) * g_camera.fLengthIntervalCamera + g_camera.posR.x;

	// sin(回転量) * カメラの視点と注視点の間の距離を算出 + 注視点のZ
	Z = sin( Model -> rot.y + D3DXToRadian( -90 ) ) * g_camera.fLengthIntervalCamera + g_camera.posR.z;

	g_camera.posP = D3DXVECTOR3( X, g_camera.posP.y, Z );
}

//============================================================================
//
// カメラの情報取得 関数
// 引数：
//　なし
// 戻り値：
//　g_camera
//
//============================================================================

CAMERA GetCamera( void )
{
	return g_camera;
}

/* EOF */