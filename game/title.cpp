//===============================================================================================================
//
// title.cpp
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
#include "title.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

void MakeVertexTitle(void);

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureTitle    = NULL;		// テクスチャへのポインタ(Title用)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTitle    = NULL;		// 頂点バッファインターフェースへのポインタ(Title用)

static LPDIRECT3DTEXTURE9		g_pD3DTextureTitle_2  = NULL;		// テクスチャへのポインタ(Title用)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTitle_2  = NULL;		// 頂点バッファインターフェースへのポインタ(Title用)

static LPDIRECT3DTEXTURE9		g_pD3DTextureEnter    = NULL;		// テクスチャへのポインタ(Enter用)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffEnter    = NULL;		// 頂点バッファインターフェースへのポインタ(Enter用)

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitTitle(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 頂点情報の作成
	MakeVertexTitle();

	HRESULT hr;

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_TITLE_FILE,		// ファイルの名前
									&g_pD3DTextureTitle);	// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_TITLE_2_FILE,	// ファイルの名前
									&g_pD3DTextureTitle_2);	// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_ENTER_FILE,		// ファイルの名前
									&g_pD3DTextureEnter);	// 読み込むメモリー

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	return S_OK;
}

//============================================================================
//
// 頂点情報の作成 関数
// 引数：
//　なし
// 戻り値：
//	頂点情報の設定
//	 S_OK	: 頂点情報の設定に成功
//	 E_FAIL	: 頂点情報の設定に失敗
//　
//============================================================================

void MakeVertexTitle(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffTitle,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffTitle_2,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

    D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffEnter,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定
	
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTitle -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	// 頂点データをアンロックする
	g_pD3DVtxBuffTitle -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTitle_2 -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	// 頂点データをアンロックする
	g_pD3DVtxBuffTitle_2 -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffEnter -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	// 頂点データをアンロックする
	g_pD3DVtxBuffEnter -> Unlock();
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

void UpdateTitle(void)
{
	;
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

void DrawTitle(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	int nTime = GetTime();

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	if( nTime % 2 == 0 )
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTitle, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureTitle );

		// ポリゴンの描画
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );

		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffEnter, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureEnter );

		// ポリゴンの描画
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
	}
	else
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTitle_2, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureTitle_2 );

		// ポリゴンの描画
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
	}
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

void UninitTitle(void)
{
	// テクスチャの開放
	if( g_pD3DTextureTitle != NULL )
	{
		g_pD3DTextureTitle -> Release();
		g_pD3DTextureTitle = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffTitle != NULL )
	{
		g_pD3DVtxBuffTitle -> Release();
		g_pD3DVtxBuffTitle = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureTitle_2 != NULL )
	{
		g_pD3DTextureTitle_2 -> Release();
		g_pD3DTextureTitle_2 = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffTitle_2 != NULL )
	{
		g_pD3DVtxBuffTitle_2 -> Release();
		g_pD3DVtxBuffTitle_2 = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureEnter != NULL )
	{
		g_pD3DTextureEnter -> Release();
		g_pD3DTextureEnter = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffEnter != NULL )
	{
		g_pD3DVtxBuffEnter -> Release();
		g_pD3DVtxBuffEnter = NULL;
	}
}

/* EOF */