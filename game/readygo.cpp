//===============================================================================================================
//
// readygo.cpp
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
#include "game.h"
#include "readygo.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

void MakeVertexReadGo(void);

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureRead = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffRead = NULL;		// 頂点バッファインターフェースへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3DTextureGo   = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffGo   = NULL;		// 頂点バッファインターフェースへのポインタ

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitReadGo(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_READY_FILE,		// ファイルの名前
									&g_pD3DTextureRead);	// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_GO_FILE,		// ファイルの名前
									&g_pD3DTextureGo);		// 読み込むメモリー

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	// 頂点情報の作成
	MakeVertexReadGo();

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

void MakeVertexReadGo(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffRead,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	// オブジェクトの頂点バッファを生成
    D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffGo,			// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定
	
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffRead -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffRead -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGo -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffGo -> Unlock();
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

void UpdateReadGo(void)
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

void DrawReadGo(void)
{
	int nTime = GetGameTime();

	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	if( nTime == TIME_LIMIT )
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffRead, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureRead );

		// ポリゴンの描画
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );
	}

	if( nTime == TIME_LIMIT - 1 )
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffGo, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureGo );

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

void UninitReadGo(void)
{
	// テクスチャの開放
	if( g_pD3DVtxBuffRead != NULL )
	{
		g_pD3DVtxBuffRead -> Release();
		g_pD3DVtxBuffRead = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DTextureRead != NULL )
	{
		g_pD3DTextureRead -> Release();
		g_pD3DTextureRead = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureGo != NULL )
	{
		g_pD3DTextureGo -> Release();
		g_pD3DTextureGo = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffGo != NULL )
	{
		g_pD3DVtxBuffGo -> Release();
		g_pD3DVtxBuffGo = NULL;
	}
}

/* EOF */