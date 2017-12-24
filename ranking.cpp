//===============================================================================================================
//
// ranking.cpp
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
#include "number.h"
#include "ranking.h"
#include "title.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

void MakeVertexRanking(void);

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureRanking = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffRanking = NULL;		// 頂点バッファインターフェースへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3DTextureSecond = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffSecond = NULL;			// 頂点バッファインターフェースへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3DTextureEnter    = NULL;		// テクスチャへのポインタ(Enter用)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffEnter    = NULL;		// 頂点バッファインターフェースへのポインタ(Enter用)

static int g_nTempTime;

static D3DXVECTOR2 g_RankingPos;				// ポジション

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitGameRecord(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_nTempTime = 0;

	g_RankingPos = D3DXVECTOR2(0.0f,0.0f);

	// 頂点情報の作成
	MakeVertexRanking();

	HRESULT hr;

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_RANKING_FILE,		// ファイルの名前
									&g_pD3DTextureRanking);		// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_SECOND_FILE,		// ファイルの名前
									&g_pD3DTextureSecond);		// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_ENTER_FILE,			// ファイルの名前
									&g_pD3DTextureEnter);		// 読み込むメモリー

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

void MakeVertexRanking(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffRanking,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffSecond,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffEnter,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffRanking -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3(0, 0, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(0, 0, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(0, 0, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(0, 0, 0.f);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	// 頂点データをアンロックする
	g_pD3DVtxBuffRanking -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffSecond -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3(0, 0, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	// 頂点データをアンロックする
	g_pD3DVtxBuffSecond -> Unlock();

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

void UpdateGameRecord(void)
{
	int nTime = GetTime();

	if( nTime %2 == 0 )
	{
		g_nTempTime++;
	}
/*
	float PosX_1 = g_RankingPos.x - TEX_RANKING_SIZE_X;
	float PosX_2 = g_RankingPos.x + TEX_RANKING_SIZE_X;
	float PosY_1 = g_RankingPos.y - TEX_RANKING_SIZE_Y;
	float PosY_2 = g_RankingPos.y + TEX_RANKING_SIZE_Y;
*/
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffRanking -> Lock( 0, 0, (void**)&pVtx, 0 );
/*
	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3(PosX_1, PosY_1, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(PosX_2, PosY_1, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(PosX_1, PosY_2, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(PosX_2, PosY_2, 0.f);
*/
	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3( 0, 0, 0.f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0, 0.f );
	pVtx[2].vtx = D3DXVECTOR3( 0, SCREEN_HEIGHT, 0.f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.f );

	pVtx[0].rhw = pVtx[1].rhw = pVtx[2].rhw = pVtx[3].rhw = 1.f;

	pVtx[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	// 頂点データをアンロックする
	g_pD3DVtxBuffRanking -> Unlock();
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

void DrawGameRecord(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffSecond, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	D3DDevice -> SetTexture( 0, g_pD3DTextureSecond );

	// ポリゴンの描画
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	// 頂点バッファをデバイスのデータストリームにバインド
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffRanking, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	D3DDevice -> SetTexture( 0, g_pD3DTextureRanking );

	// ポリゴンの描画
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	int nTime = GetTime();

	if( g_nTempTime >= 30 )
	{
		if( nTime %2 == 0 )
		{
			// 頂点バッファをデバイスのデータストリームにバインド
			D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffEnter, 0, sizeof( VERTEX_2D ) );

			// テクスチャの設定
			D3DDevice -> SetTexture( 0, g_pD3DTextureEnter );

			// ポリゴンの描画
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );
		}
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

void UninitGameRecord(void)
{
	// テクスチャの開放
	if( g_pD3DTextureRanking != NULL )
	{
		g_pD3DTextureRanking -> Release();
		g_pD3DTextureRanking = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffRanking != NULL )
	{
		g_pD3DVtxBuffRanking -> Release();
		g_pD3DVtxBuffRanking = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureSecond != NULL )
	{
		g_pD3DTextureSecond -> Release();
		g_pD3DTextureSecond = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffSecond != NULL )
	{
		g_pD3DVtxBuffSecond -> Release();
		g_pD3DVtxBuffSecond = NULL;
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