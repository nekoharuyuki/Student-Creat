//===============================================================================================================
//
// number.cpp
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
#include "number.h"
#include "game.h"

//============================================================================
//
// マクロ定義
//
//============================================================================

// 数字１個分のテクスチャサイズ
#define	TEXTURE_SIZE_X		( 448.0f )
#define	TEXTURE_SIZE_Y		( 289.0f )

// 頂点数
#define NUM_NUMBER_POLYGON  ( 2 )
#define NUM_NUMBER_VERTEX   ( 4 )

// 数字の種類
#define TEXTURE_DIV_X       ( 10 )
#define	NUM_NUMBERTYPE		( 4 )

//// 数字のサイズ
#define NUMBER_SIZE_X       ( TEXTURE_SIZE_X / TEXTURE_DIV_X  )
#define NUMBER_SIZE_Y       ( TEXTURE_SIZE_Y / NUM_NUMBERTYPE )

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexNumber(void);
void SetVertexNumber(int nIdxNumber);
void SetTextureNumber(int nIdxNumber);
void SetColorNumber(int nIdxNumber, D3DXCOLOR col);

//============================================================================
//
// グローバル変数
//
//============================================================================

// 数字
static LPDIRECT3DTEXTURE9		g_pD3DTextureNumber = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffNumber = NULL;			// 頂点バッファインターフェースへのポインタ

// フレーム
static LPDIRECT3DTEXTURE9		g_pD3DTextureFrame = NULL;			// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9  g_pD3DVtxBuffFrame = NULL;			// 頂点バッファインターフェースへのポインタ

static NUMBER					g_aNumber[ MAX_NUM_NUMBER ];		// 数字のワーク

// スコア用変数
static int g_score = 0;

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//　HRESULT hr
//
//============================================================================

HRESULT InitNumber(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 数字の初期化
	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		g_aNumber[ nCntNumber ].pos    = D3DXVECTOR3( NUMBER_POS_X, NUMBER_POS_Y, 0.0f );
		g_aNumber[ nCntNumber ].rot    = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aNumber[ nCntNumber ].nValue = 0;
		g_aNumber[ nCntNumber ].nType  = 0;
		g_aNumber[ nCntNumber ].bUse   = false;
	}

	// スコア用変数の初期化
	g_score = 0;

	// 頂点情報の作成
	MakeVertexNumber();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
							   TEXTURE_NUMBER,			// ファイルの名前
							   &g_pD3DTextureNumber );	// 読み込むメモリー

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

void UpdateNumber(void)
{
    int nDivision;
    int nRemainder;

    nRemainder = g_score % 10;
    nDivision  = g_score / 10;

    int nCntNumber = 0;

	GAMEMODE Mode = GetMode();

	float x;
	float y;

	if( Mode == SCENE_GAME )
	{
		x = NUMBER_POS_X;
		y = NUMBER_POS_Y;
	}
	else
	{
		if( Mode == SCENE_RECORD )
		{
			x = SCREEN_WIDTH  / 2;
			y = SCREEN_HEIGHT / 2;
		}
		else
		{
			x = 0;
			y = 0;
		}
	}

    do
    {
		// 数字の作成
		g_aNumber[ nCntNumber ].nValue = nRemainder;				// 値
		g_aNumber[ nCntNumber ].bUse   = true;						// 使用中フラグ
		g_aNumber[ nCntNumber ].pos    = D3DXVECTOR3( x, y, 0.0f );	// ポジション

		// 頂点の設定
		SetVertexNumber( nCntNumber );

		// テクスチャの設定
		SetTextureNumber( nCntNumber );

		nCntNumber++;

		x -= NUMBER_SIZE_X + 20;

		nRemainder = nDivision % 10;
		nDivision  = nDivision / 10;

    }while( nDivision > 0 || nRemainder > 0 );
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

void DrawNumber(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    D3DDevice -> SetStreamSource( 0, g_pD3DVtxBuffNumber, 0, sizeof( VERTEX_2D ) );

	// 頂点フォーマットの設定
	D3DDevice -> SetFVF( FVF_VERTEX_2D );

	// テクスチャの設定
	D3DDevice -> SetTexture( 0, g_pD3DTextureNumber );

	// ポリゴンの描画
	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		if( g_aNumber[ nCntNumber ].bUse )
		{
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP, ( nCntNumber * 4 ), NUM_NUMBER_POLYGON );
		}
	}

	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++ )
	{
		g_aNumber[nCntNumber].pos    = D3DXVECTOR3( NUMBER_POS_X, NUMBER_POS_Y, 0.0f );
		g_aNumber[nCntNumber].rot    = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aNumber[nCntNumber].nValue = 0;
		g_aNumber[nCntNumber].nType  = 0;
		g_aNumber[nCntNumber].bUse   = false;
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

void UninitNumber(void)
{
	// テクスチャの開放
	if( g_pD3DTextureNumber != NULL )
	{
		g_pD3DTextureNumber -> Release();
		g_pD3DTextureNumber = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffNumber != NULL )
	{
		g_pD3DVtxBuffNumber -> Release();
		g_pD3DVtxBuffNumber = NULL;
	}
	// テクスチャの開放
	if( g_pD3DTextureFrame != NULL )
	{
		g_pD3DTextureFrame -> Release();
		g_pD3DTextureFrame = NULL;
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffFrame != NULL )
	{
		g_pD3DVtxBuffFrame -> Release();
		g_pD3DVtxBuffFrame = NULL;
	}
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

HRESULT MakeVertexNumber(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
    if( FAILED( D3DDevice->CreateVertexBuffer( sizeof(VERTEX_2D) * NUM_NUMBER_VERTEX * MAX_NUM_NUMBER,	// 頂点データ用に確保するバッファサイズ(バイト単位)
											   D3DUSAGE_WRITEONLY,										// 頂点バッファの使用法　
											   FVF_VERTEX_3D,											// 使用する頂点フォーマット
											   D3DPOOL_MANAGED,											// リソースのバッファを保持するメモリクラスを指定
											   &g_pD3DVtxBuffNumber,									// 頂点バッファインターフェースへのポインタ
											   NULL)))													// NULLに設定
	{
        return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber -> Lock( 0, 0, (void**)&pVtx, 0 );

	for( int nCntNumber = 0; nCntNumber < MAX_NUM_NUMBER; nCntNumber++, pVtx += 4 )
	{
		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x, g_aNumber[ nCntNumber ].pos.y, 0.0f );
		pVtx[1].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[nCntNumber].pos.y, 0.0f );
		pVtx[2].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x, g_aNumber[nCntNumber].pos.y + NUMBER_SIZE_Y, 0.0f );
		pVtx[3].vtx = D3DXVECTOR3( g_aNumber[ nCntNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[nCntNumber].pos.y + NUMBER_SIZE_Y, 0.0f );

		// rhwの設定
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[1].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[2].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		pVtx[3].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2( 0.0f,           0.0f );
		pVtx[1].tex = D3DXVECTOR2( TEXTURE_SIZE_X, 0.0f );
		pVtx[2].tex = D3DXVECTOR2( 0.0f,           TEXTURE_SIZE_Y );
		pVtx[3].tex = D3DXVECTOR2( TEXTURE_SIZE_X, TEXTURE_SIZE_Y );
	}

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber -> Unlock();

	return S_OK;
}

//============================================================================
//
// 頂点の設定 関数
// 引数：
//　int nIdxNumber
// 戻り値：
//	なし
//　
//============================================================================

void SetVertexNumber(int nIdxNumber)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber -> Lock( 0, 0, ( void** )&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x, g_aNumber[ nIdxNumber ].pos.y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[ nIdxNumber ].pos.y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x, g_aNumber[ nIdxNumber ].pos.y + NUMBER_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3( g_aNumber[ nIdxNumber ].pos.x + NUMBER_SIZE_X, g_aNumber[ nIdxNumber ].pos.y + NUMBER_SIZE_Y, 0.0f);

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// テクスチャの設定 関数
// 引数：
//　int nIdxNumber
// 戻り値：
//	なし
//　
//============================================================================

void SetTextureNumber(int nIdxNumber)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber -> Lock( 0, 0, ( void** )&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// 頂点座標の設定
    float x1, x2;
    x1 = 1.0f * g_aNumber[ nIdxNumber ].nValue / TEXTURE_DIV_X;
    x2 = 1.0f * ( g_aNumber[ nIdxNumber ].nValue + 1 ) / TEXTURE_DIV_X;

	pVtx[ 0 ].tex = D3DXVECTOR2( x1, 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( x2, 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( x1, 1.0f / NUM_NUMBERTYPE ); 
	pVtx[ 3 ].tex = D3DXVECTOR2( x2, 1.0f / NUM_NUMBERTYPE );

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// カラーの設定 関数
// 引数：
//　int nIdxNumber
//	D3DXCOLOR col
// 戻り値：
//	なし
//　
//============================================================================

void SetColorNumber(int nIdxNumber, D3DXCOLOR col)
{
	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffNumber -> Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx += nIdxNumber * 4;

	// 頂点座標の設定
	pVtx[0].diffuse = 
	pVtx[1].diffuse = 
	pVtx[2].diffuse = 
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffNumber -> Unlock();
}

//============================================================================
//
// スコアの設定 関数
// 引数：
//　int n
// 戻り値：
//	なし
//　
//============================================================================

void SetScore(int n)
{
    g_score = n;
}

/* EOF */