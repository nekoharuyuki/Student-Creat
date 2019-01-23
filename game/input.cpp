//===============================================================================================================
//
// input.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// インクルード
//
//============================================================================

#include "DirectX3D.h"
#include "input.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

// DirectInputの変数
static LPDIRECTINPUT8 g_pDInput = NULL;			// DirectInput
static LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;	// DirectInputデバイス

static BYTE g_Key[ 256 ] = {NULL};				// 入力情報を受けとる配列
static BYTE g_oldKey[ 256 ] = {NULL};			// 入力情報を受けとる配列

//============================================================================
//
// DirectInput 初期化関数
// 引数：
//　hWnd    　　　…  ウインドウのハンドル
//　hInstance     …　このプログラムを識別するためのハンドル
// 戻り値：
//　DirectInput 初期化の処理結果
//
//============================================================================

HRESULT InitDInput( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;

	// DirectInputの作成
	hr = DirectInput8Create( hInstance,				// インスタンスハンドル
							 DIRECTINPUT_VERSION, 
							 IID_IDirectInput8,
							 ( void** )&g_pDInput,	// DirectInputオブジェクトを受けとるポインタ変数
							 NULL ); 
	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// DirectInput9オブジェクトの作成に失敗
	}

	// デバイス・オブジェクトを作成
	// キーボード用デバイスオブジェクトを生成
	hr = g_pDInput -> CreateDevice( GUID_SysKeyboard,
									&g_pDIDevice,
									NULL ); 
	if( FAILED( hr ) ) 
	{
	    return E_FAIL;	// DirectInputDevice9オブジェクトの作成に失敗
	}

	// データ形式を設定
	hr = g_pDIDevice -> SetDataFormat( &c_dfDIKeyboard );
	if( FAILED( hr ) )
	{
		return E_FAIL;	// データフォーマットの設定に失敗
	}

	//モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevice -> SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		return E_FAIL;	// フォアグラウンド＆非排他モードの設定に失敗
	}

	// キーボードのアクセス権を取得
	g_pDIDevice -> Acquire();

	return S_OK;
}

//============================================================================
//
// DirectInputの更新 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UpdataDInput( void )
{
	// 過去のキー保存
	for( int nLoop = 0; nLoop < 256; nLoop++ )
	{
		g_oldKey[ nLoop ] = g_Key[ nLoop ];
	}

	HRESULT hr;

	// デバイスの直接データを取得する
	hr = g_pDIDevice -> GetDeviceState( sizeof( g_Key ), &g_Key[ 0 ] );

	if( SUCCEEDED( hr ) )
	{
	}
	else
	{
		// キーボードのアクセス権を取得
		g_pDIDevice ->Acquire();
	}
}

//============================================================================
//
// キー入力の判定 関数
// 引数：
//　なし
// 戻り値：
//　押しているとき　…　ture　/　押していないとき　…　false
//
//============================================================================

bool nowKey( BYTE cKey )
{
	if( g_Key[ cKey ] && 0x80 )
	{	// 押している
		// 処理
		return true;
	}
	else
	{	// 押していない
		return false;
	}
}

//============================================================================
//
// キー入力の判定 関数(押した瞬間)
// 引数：
//　なし
// 戻り値：
//　押した瞬間　…　ture　/　押していないとき　…　false
//
//============================================================================

bool PushKey( BYTE cKey )
{
	if( ( ( g_Key[ cKey ] ^ g_oldKey[ cKey ] ) &  g_Key[ cKey ] ) & 0x80 )
	{	// 押した瞬間
		// 処理
		return true;
	}
	else
	{	// 押していない
		return false;
	}
}

//============================================================================
//
// キー入力の判定 関数(離した瞬間)
// 引数：
//　なし
// 戻り値：
//　離した瞬間　…　ture　/　押していないとき　…　false
//
//============================================================================

bool ReleaseKey( BYTE cKey )
{
	if( ( ( g_Key[ cKey ] ^ g_oldKey[ cKey ] ) & ~g_Key[ cKey ] ) & 0x80 )
	{	// 離した瞬間
		// 処理
		return true;
	}
	else
	{	// 押していない
		return false;
	}
}

//============================================================================
//
// DirectInputデバイスとオブジェクトの開放 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void ReleaseDInput( void )
{
	if( NULL != g_pDIDevice )
	{	// デバイスの開放
		g_pDIDevice -> Unacquire();
		g_pDIDevice -> Release();
		g_pDIDevice = NULL;
	}

	if( NULL != g_pDInput )
	{	// オブジェクトの開放
		g_pDInput -> Release();
		g_pDInput = NULL;
	}
}

/* EOF */