//===============================================================================================================
//
// gameover.cpp
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
#include "gameover.h"
#include "title.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

void MakeVertexGameOver(void);

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPDIRECT3DTEXTURE9		g_pD3DTextureGameOver = NULL;		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffGameOver = NULL;		// 頂点バッファインターフェースへのポインタ

static LPDIRECT3DTEXTURE9		g_pD3DTextureCryPig   = NULL;		// テクスチャへのポインタ（ブタ）
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffCryPig   = NULL;		// 頂点バッファインターフェースへのポインタ（ブタ）

static LPDIRECT3DTEXTURE9		g_pD3DTextureTear     = NULL;		// テクスチャへのポインタ（涙）
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffTear     = NULL;		// 頂点バッファインターフェースへのポインタ（涙）

static LPDIRECT3DTEXTURE9		g_pD3DTextureEnter    = NULL;		// テクスチャへのポインタ(Enter用)
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffEnter    = NULL;		// 頂点バッファインターフェースへのポインタ(Enter用)

static D3DXVECTOR2 g_CryPig;				// キャラクターのポジション

static float g_fRadian;			// 回転
static float g_fR;				// 半径
static float g_fCenter_X;		// 中心点X
static float g_fCenter_Y;		// 中心点Y
static float g_fRota;			// 回転量

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitGameOver(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 回転の初期化
	g_fRadian   = 0.f;	// 回転
	g_fR        = 0.f;	// 半径
	g_fCenter_X = 0.f;	// 中心点X
	g_fCenter_Y = 0.f;	// 中心点Y
	g_fRota     = 0.f;	// 回転量

	g_fRadian = atan2f( TEX_PIG_SIZE, TEX_PIG_SIZE );
	g_fR = sqrtf( ( TEX_PIG_SIZE * TEX_PIG_SIZE ) + ( TEX_PIG_SIZE * TEX_PIG_SIZE ) );

	g_CryPig = D3DXVECTOR2(0.0f, 0.0f);
	g_fCenter_X = g_CryPig.x + SCREEN_WIDTH  / 2;	// 中心点X
	g_fCenter_Y = g_CryPig.y + SCREEN_HEIGHT / 2;	// 中心点Y

	// キャラクターのポジション
	g_CryPig = D3DXVECTOR2( g_fCenter_X, g_fCenter_Y );

	// 頂点情報の作成
	MakeVertexGameOver();

	HRESULT hr;

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_GAMEOVER_FILE,		// ファイルの名前
									&g_pD3DTextureGameOver);	// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_CRYPIG_FILE,		// ファイルの名前
									&g_pD3DTextureCryPig);		// 読み込むメモリー

	hr = D3DXCreateTextureFromFile( D3DDevice,					// デバイスへのポインタ
									TEXTURE_CRY_FILE,			// ファイルの名前
									&g_pD3DTextureTear);		// 読み込むメモリー

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

void MakeVertexGameOver(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffGameOver,	// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffCryPig,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffTear,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
									 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
									 FVF_VERTEX_3D,				// 使用する頂点フォーマット
									 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
									 &g_pD3DVtxBuffEnter,		// 頂点バッファインターフェースへのポインタ
									 NULL);						// NULLに設定

	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffGameOver -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffGameOver -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffCryPig -> Lock( 0, 0, (void**)&pVtx, 0 );

	// 座標指定
	pVtx[0].vtx = D3DXVECTOR3(g_CryPig.x - TEX_PIG_SIZE, g_CryPig.y - TEX_PIG_SIZE, 0.f);
	pVtx[1].vtx = D3DXVECTOR3(g_CryPig.x + TEX_PIG_SIZE, g_CryPig.y - TEX_PIG_SIZE, 0.f);
	pVtx[2].vtx = D3DXVECTOR3(g_CryPig.x - TEX_PIG_SIZE, g_CryPig.y + TEX_PIG_SIZE, 0.f);
	pVtx[3].vtx = D3DXVECTOR3(g_CryPig.x + TEX_PIG_SIZE, g_CryPig.y + TEX_PIG_SIZE, 0.f);

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
	g_pD3DVtxBuffCryPig -> Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffTear -> Lock( 0, 0, (void**)&pVtx, 0 );

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
	g_pD3DVtxBuffTear -> Unlock();

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

void UpdateGameOver(void)
{
/*
	// 回転処理
	if( g_fRota > D3DXToRadian( 135 ) )
	{
		g_fRota -= 0.02f;			   // 回転
		Rotation( g_fRota );
	}

	if( g_fRota < D3DXToRadian( 225 ) )
	{
		g_fRota += 0.02f;			   // 回転
		Rotation( g_fRota );
	}
*/
#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ情報表示
	PrintDebugProc( "PIG POS : ( %.2f : %.2f )\n", g_CryPig.x, g_CryPig.y );
	PrintDebugProc( "PIG ROT : ( %.2f )\n", g_fRota );

#endif //_DEBUG	//デバッグ用コード終了

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

void DrawGameOver(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffGameOver, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	D3DDevice -> SetTexture( 0, g_pD3DTextureGameOver );

	// ポリゴンの描画
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	// 頂点バッファをデバイスのデータストリームにバインド
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffCryPig, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	D3DDevice -> SetTexture( 0, g_pD3DTextureCryPig );

	// ポリゴンの描画
	D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
								0,
								2 );

	int nTime = GetTime();

	if( nTime % 2 == 0 )
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
	else
	{
		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffTear, 0, sizeof( VERTEX_2D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_pD3DTextureTear );

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

void UninitGameOver(void)
{
	// テクスチャの開放
	if( g_pD3DTextureGameOver != NULL )
	{
		g_pD3DTextureGameOver -> Release();
		g_pD3DTextureGameOver = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffGameOver != NULL )
	{
		g_pD3DVtxBuffGameOver -> Release();
		g_pD3DVtxBuffGameOver = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureTear != NULL )
	{
		g_pD3DTextureTear -> Release();
		g_pD3DTextureTear = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffTear != NULL )
	{
		g_pD3DVtxBuffTear -> Release();
		g_pD3DVtxBuffTear = NULL;
	}

	// テクスチャの開放
	if( g_pD3DTextureCryPig != NULL )
	{
		g_pD3DTextureCryPig -> Release();
		g_pD3DTextureCryPig = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffCryPig != NULL )
	{
		g_pD3DVtxBuffCryPig -> Release();
		g_pD3DVtxBuffCryPig = NULL;
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

//============================================================================
//
// テクスチャの動き(回転) 関数
// 引数：
//　float Rota
// 戻り値：
//　なし
//
//============================================================================

void Rotation(float Rota)
{
	VERTEX_2D *pVtx;

	g_pD3DVtxBuffCryPig -> Lock( 0 ,0 , ( void** )&pVtx , 0 );

	g_CryPig.x = g_fCenter_X - sinf( g_fRadian - g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y - cosf( g_fRadian - g_fRota ) * g_fR;

	pVtx[0].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X + sinf( g_fRadian + g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y - cosf( g_fRadian + g_fRota ) * g_fR;

	pVtx[1].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X - sinf( g_fRadian + g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y + cosf( g_fRadian + g_fRota ) * g_fR;

	pVtx[2].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_CryPig.x = g_fCenter_X + sinf( g_fRadian - g_fRota ) * g_fR;
	g_CryPig.y = g_fCenter_Y + cosf( g_fRadian - g_fRota ) * g_fR;

	pVtx[3].vtx = D3DXVECTOR3( g_CryPig.x ,g_CryPig.y , 0.0f );

	g_pD3DVtxBuffCryPig -> Unlock();
}

/* EOF */