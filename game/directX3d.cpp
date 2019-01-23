//===============================================================================================================
//
// DirectX3D.cpp
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

//============================================================================
//
// グローバル変数
//
//============================================================================

// インターフェイス
static LPDIRECT3D9           g_pD3D       = NULL;	// Direct3Dオブジェクト
static LPDIRECT3DDEVICE9     g_pD3DDevice = NULL;	// Directオブジェクト（描画に必要）

//============================================================================
//
// DirectXの初期化 関数
// 引数：
//  void
// 戻り値：
//　DirectX 初期化の処理結果
//		S_OK	…　成功
//		E_FAIL	…　失敗
//
//============================================================================

HRESULT InitDX( HWND hWnd )
{
	HRESULT hr;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );	// Direct3D9インターフェイスの取得
	
	if( NULL == g_pD3D )
	{
		return E_FAIL;	// 取得に失敗
	}
	
	// 現在のディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	hr = g_pD3D -> GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	
	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	// デバイスのプレゼンテーションパラメータの設定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );									// ワークをゼロクリア
	d3dpp.BackBufferCount                = 1;								// バックバッファの数
	d3dpp.BackBufferWidth                = SCREEN_WIDTH;					// ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight               = SCREEN_HEIGHT;					// ゲーム画面サイズ（高さ）
	d3dpp.SwapEffect                     = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップ
	d3dpp.Windowed                       = TRUE;							// ウインドウモード
	d3dpp.EnableAutoDepthStencil         = TRUE;							// デプスバッファ（Zバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat         = D3DFMT_D16;						// 16bitをデプスバッファ（Zバッファ）として使用

	if( TRUE == d3dpp.Windowed )
	{	// ウインドウモード
		d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	}
	else
	{	// フルスクリーンモード
		d3dpp.BackBufferFormat           = D3DFMT_R5G6B5;					// バックバッファ
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// リフレッシュレート
		d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル
	}

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	struct{
		D3DDEVTYPE DevType;
		DWORD BehaviorFlags;
	} DevTypes [] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// 描画と頂点処理をハードウェア計算
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// 描画をハードウェア，頂点処理をCPU
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// 描画をCPU，頂点処理をハードウェア
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// 描画と頂点処理をCPU計算
	};
	
	D3DDEVTYPE DevType;
	DWORD BehaviorFlags;
	BOOL bCreateDevice;
	bCreateDevice = FALSE;
	
	for( int nLoop = 0; nLoop < _countof( DevTypes ); nLoop++ )
	{
		DevType       = DevTypes[ nLoop ].DevType;
		BehaviorFlags = DevTypes[ nLoop ].BehaviorFlags;
		hr = g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT,	// ディスプレイアダプタ
									 DevType,				// ディスプレイタイプ
									 hWnd,					// ウインドウハンドル
									 BehaviorFlags,			// デバイス作成制御の組み合わせ
									 &d3dpp,				// プレゼンテーションパラメータ
									 &g_pD3DDevice );		// デバイスインターフェースポインタ

		if( SUCCEEDED( hr ) )
		{	// デバイス作成成功したならば、ループを抜ける
			bCreateDevice = TRUE;
			break;
		}
	}
	
	if( FALSE == bCreateDevice )
	{	// デバイス作成失敗
		return( E_FAIL );
	}

	// レンダリングステートパラメータの設定
	g_pD3DDevice -> SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// カリングを行う
	g_pD3DDevice -> SetRenderState( D3DRS_ZENABLE, TRUE );						// Zバッファを使用
	g_pD3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			// αブレンドを使用
	g_pD3DDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// αソースカラーの指定
	g_pD3DDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// αディスティネーションカラーの指定
	
	// サンプラーステートパラメータの設定
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );	// テクスチャU値の繰返設定
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );	// テクスチャV値の繰返設定
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// テクスチャ拡大時の補間設定
	g_pD3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// テクスチャ拡小時の補間設定

	return S_OK;
}

//============================================================================
//
// DirectXデバイスとオブジェクトの開放 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void ReleaseD3D( void )
{
	if( NULL != g_pD3DDevice )
	{	// デバイスの開放
		g_pD3DDevice -> Release();
		g_pD3DDevice = NULL;
	}
	
	if( NULL != g_pD3D )
	{	// Direct3Dオブジェクトの開放
		g_pD3D -> Release();
		g_pD3D = NULL;
	}
}

//============================================================================
//
// DirectXデバイスのゲッター 関数
// 引数：
//　なし
// 戻り値：
//　g_pD3DDevice
//
//============================================================================

LPDIRECT3DDEVICE9 GetD3DDevice( void )
{
	return g_pD3DDevice;
}

/* EOF */