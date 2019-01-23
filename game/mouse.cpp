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

#include "main.h"
#include "DirectX3D.h"
#include "debugproc.h"
#include "input.h"
#include "mouse.h"

#include "model.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

// DirectInputの変数
static LPDIRECTINPUT8 g_pDInput = NULL;			// DirectInput
static LPDIRECTINPUTDEVICE8 g_pDIDevice = NULL;	// DirectInputデバイス
#define DIDEVICE_BUFFERSIZE	100					// デバイスに設定するバッファ・サイズ

static DIMOUSESTATE2 g_Mouse;		// 入力情報を受けとる
static DIMOUSESTATE2 g_OldMouse;	// 入力情報を受けとる
static MOUSE MouseState;			// マウスポインタ情報

//============================================================================
//
// DirectMouse 初期化関数
// 引数：
//　hWnd    　　　…  ウインドウのハンドル
//　hInstance     …　このプログラムを識別するためのハンドル
// 戻り値：
//　DirectMouse 初期化の処理結果
//
//============================================================================

HRESULT InitDMouse( HINSTANCE hInstance, HWND hWnd )
{
	HRESULT hr;

	// マウスポインタ情報の初期化
	MouseState.x = SCREEN_WIDTH  / 2;									// マウスカーソルのＸ座標を初期化
	MouseState.y = SCREEN_HEIGHT / 2;									// マウスカーソルのＹ座標を初期化
	MouseState.lButton = false;							// 左ボタンの情報を初期化
	MouseState.rButton = false;							// 右ボタンの情報を初期化
	MouseState.cButton = false;							// 中央ボタンの情報を初期化

	// DirectInputの作成
	hr = DirectInput8Create( hInstance,				// インスタンスハンドル
							 DIRECTINPUT_VERSION, 
							 IID_IDirectInput8,
							 ( void** )&g_pDInput,	// DirectInputオブジェクトを受けとるポインタ変数
							 NULL ); 
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
		return E_FAIL;	// DirectInput9オブジェクトの作成に失敗
	}

	// デバイス・オブジェクトを作成
	// キーボード用デバイスオブジェクトを生成
	hr = g_pDInput -> CreateDevice( GUID_SysMouse,
									&g_pDIDevice,
									NULL ); 
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
	    return E_FAIL;	// DirectInputDevice9オブジェクトの作成に失敗
	}

	// データ形式を設定
	hr = g_pDIDevice -> SetDataFormat(&c_dfDIMouse2);
	if( FAILED( hr ) ) 
	{
		MouseState.bUse = false;
	    return E_FAIL;	// データ形式の設定に失敗
	}

	//モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevice -> SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		MouseState.bUse = false;
		return E_FAIL;	// フォアグラウンド＆非排他モードの設定に失敗
	}

	// 軸モードを設定（相対値モードに設定）
	DIPROPDWORD diprop;
	diprop.diph.dwSize	     = sizeof(diprop); 
	diprop.diph.dwHeaderSize = sizeof(diprop.diph); 
	diprop.diph.dwObj	     = 0;
	diprop.diph.dwHow	     = DIPH_DEVICE;
	diprop.dwData		     = DIPROPAXISMODE_REL;
//	diprop.dwData		     = DIPROPAXISMODE_ABS;	// 絶対値モードの場合

	hr = g_pDIDevice -> SetProperty( DIPROP_AXISMODE, &diprop.diph );

	if( FAILED( hr ) )
	{
		MouseState.bUse = false;
		return E_FAIL;	// 軸モードの設定に失敗
	}

	// キーボードのアクセス権を取得
	g_pDIDevice -> Acquire();
	MouseState.bUse = true;

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

void UpdataDMouse( HWND hWnd )
{
	// 過去のキー保存
	for( int nLoop = 0; nLoop < 8; nLoop++ )
	{
		g_OldMouse.rgbButtons[nLoop] = g_Mouse.rgbButtons[nLoop];
	}

	HRESULT hr;

	// デバイスの直接データを取得する
	hr = g_pDIDevice -> GetDeviceState( sizeof( DIMOUSESTATE2 ), &g_Mouse );

	if( SUCCEEDED( hr ) )
	{
		MouseState.bUse = true;
	}
	else
	{
		MouseState.bUse = false;

		// マウスのアクセス権を取得
		g_pDIDevice->Acquire();
	}

	POINT pos = {0};

	// スクリーン座標を取得
	GetCursorPos(&pos);

	// クライアント領域に変換
	ScreenToClient(hWnd, &pos);

	// 取得した情報を元にマウスの情報を更新
	// スクリーンの中心が（ 0, 0 ）
	MouseState.x = (float)pos.x - SCREEN_WIDTH  / 2;
	MouseState.y = (float)-pos.y + SCREEN_HEIGHT / 2;

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ情報表示
	PrintDebugProc("Mouse POS : ( %.2f : %.2f )\n", MouseState.x, MouseState.y);

#endif //_DEBUG	//デバッグ用コード終了

}

//============================================================================
//
// キー入力の判定 関数
// 引数：
//　なし
// 戻り値：
//  なし
//
//　押しているとき　…　ture　/　押していないとき　…　false
//
//============================================================================

bool nowClick( int nClick )
{
	(g_Mouse.rgbButtons[nClick] & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
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
//  なし
//　押した瞬間　…　ture　/　押していないとき　…　false
//
//============================================================================

bool PushClick( int nClick )
{
	( ( ( g_Mouse.rgbButtons[nClick] ^ g_OldMouse.rgbButtons[nClick] ) & g_Mouse.rgbButtons[nClick] ) & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
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
// キー入力の判定 関数(離した瞬間)
// 引数：
//　なし
// 戻り値：
//　離した瞬間　…　ture　/　押していないとき　…　false
//
//============================================================================

bool ReleaseClick( int nClick )
{
	( ( ( g_Mouse.rgbButtons[nClick] ^ g_OldMouse.rgbButtons[nClick] ) & ~g_Mouse.rgbButtons[nClick] ) & 0x80) ? MouseState.lButton = true : MouseState.lButton = false;

	if( MouseState.lButton == true )
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
// DirectInputデバイスとオブジェクトの開放 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void ReleaseDMouse( void )
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

//============================================================================
//
// DirectInputデバイスの情報取得 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

MOUSE GetMouseState(void)
{
	return MouseState;
}

/* EOF */