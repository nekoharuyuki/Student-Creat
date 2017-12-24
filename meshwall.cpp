//===============================================================================================================
//
// mashwall.cpp
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
#include "meshwall.h"
#include "meshfield.h"
#include "meshceiling.h"

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	壁の設定
//	 S_OK	: 壁の設定に成功
//	 E_FAIL	: 壁の設定に失敗
//
//============================================================================

HRESULT InitWall( void )
{
	HRESULT hr;

	hr = InitWallUp();
	hr = InitWallDown();
	hr = InitWallLeft();
	hr = InitWallRight();

	hr = InitSky();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// 設定に失敗
	}

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

void UpdataWall( void )
{
	UpdataWallUp();
	UpdataWallDown();
	UpdataWallLeft();
	UpdataWallRight();

	UpdataSky();
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

void DrawWall( void )
{
	DrawWallUp();
	DrawWallDown();
	DrawWallLeft();
	DrawWallRight();

	DrawSky();
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

void UnInitWall( void )
{
	UnInitWallUp();
	UnInitWallDown();
	UnInitWallLeft();
	UnInitWallRight();

	UnInitSky();
}

/* EOF */