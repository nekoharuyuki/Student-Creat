//===============================================================================================================
//
// debugproc.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// インクルード
//
//============================================================================

#include "main.h"
#include "input.h"
#include "directX3d.h"
#include "debugproc.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPD3DXFONT	g_pD3DXFont = NULL;				// フォントオブジェクトポインタ
static char g_aStrDebug[ MAX_FONT ] = {"\0"};		// デバッグ情報バッファ
static bool g_bDrawTex = false;						// 描画フラグ

//============================================================================
//
// デバック情報 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitDebugProc( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// フォントの設定
	HRESULT hr;
	hr = D3DXCreateFont( D3DDevice,
						 FONT_SIZE,					// 文字サイズ
						 0,
						 0,
						 0,
						 FALSE, SHIFTJIS_CHARSET,
						 OUT_DEFAULT_PRECIS,
						 DEFAULT_QUALITY,
						 DEFAULT_PITCH,
						 "Terminal",
						 &g_pD3DXFont );

	// デバック情報のクリア
	for( int nLoop = 0; nLoop < MAX_FONT; nLoop++ )
	{
		g_aStrDebug[ nLoop ] = '\0';
	}

	// 描画しない設定
	g_bDrawTex = false;

	return hr;
}

//============================================================================
//
// デバック情報の描画処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void DrawDebugProc(void)
{
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	// キー入力判定
	if( PushKey( DIK_F1 ) )
	{
		if( g_bDrawTex == false )
		{	// 描画する
			g_bDrawTex = true;
		}
		else if( g_bDrawTex == true )
		{	// 描画しない
			g_bDrawTex = false;
		}
	}

	if( g_bDrawTex == true )
	{	// 描画する
		g_pD3DXFont -> DrawText( NULL,
							 g_aStrDebug,
							 -1,
							 &rect,
							 DT_LEFT,
							 D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff ) );
	}
	
	// デバッグ情報バッファのクリア
	for( int nLoop = 0; nLoop < MAX_FONT; nLoop++ )
	{
		g_aStrDebug[ nLoop ] = '\0';
	}
}

//============================================================================
//
// デバック情報の更新処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UpdataDebugProc(void)
{
	;
}

//============================================================================
//
// デバック情報の開放処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UnInitDebugProc( void )
{
	// フォントオブジェクト解放
	if( NULL != g_pD3DXFont )
	{	// デバイスの開放
		g_pD3DXFont -> Release();
		g_pD3DXFont = NULL;
	}
}

//============================================================================
//
// デバック情報の更新処理 関数
// 引数：
//　char *fmt, ...		可変引数
// 戻り値：
//　なし
//
//============================================================================

void PrintDebugProc( char *fmt, ... )
{
	char cTemp[ MAX_FONT ];
	va_list pArg;
	va_start( pArg, fmt );
	vsprintf_s( cTemp, sizeof( cTemp ), fmt, pArg );

	strcat_s( &g_aStrDebug[ 0 ], sizeof( g_aStrDebug ), &cTemp[ 0 ]);

	va_end( pArg );
}

//va_list				可変長引数を操作するために必要な構造体
//va_start (ap, v)		可変長引数を操作する上で必要なデータを取得するマクロ
//va_arg (list, mode)	可変長引数から、順に引数を取り出すマクロ
//va_end (list)			可変長引数の操作を終了するマクロ

//============================================================================
//
// デバック情報描画フラグの取得 処理 関数
// 引数：
//　なし
// 戻り値：
//　g_bDrawTex
//
//============================================================================

bool DebugProcFlg( void )
{
	return g_bDrawTex;
}