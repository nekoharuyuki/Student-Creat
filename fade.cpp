//===============================================================================================================
//
// fade.cpp
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
#include "input.h"
#include "fade.h"
#include "game.h"

//============================================================================
//
// マクロ定義
//
//============================================================================

#define	FADE_RATE		(0.05f)		// フェード係数

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexFade(void);
void SetColor(D3DCOLOR col);

//============================================================================
//
// グローバル変数
//
//============================================================================

static LPDIRECT3DTEXTURE9      g_p3DTextureFade  = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFade = NULL;	// 頂点バッファインターフェースへのポインタ
static D3DXCOLOR               g_color;

static FADE					   g_fade     = FADE_IN;
static FADE                    g_fadeNext = FADE_IN;

static GAMEMODE                g_nextGameMode = SCENE_TITLE;

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	 S_OK	: 初期化に成功
//	 E_FAIL	: 初期化に失敗
//
//============================================================================

HRESULT InitFade(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_fade     = FADE_IN;
	g_fadeNext = FADE_IN;

	g_nextGameMode = SCENE_TITLE;

	// 頂点情報の設定
	MakeVertexFade();

	HRESULT hr;

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice,				// デバイスへのポインタ
									TEXTURE_FADE_FILE,		// ファイルの名前
									&g_p3DTextureFade );	// 読み込むメモリー

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	return S_OK;
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

void UninitFade(void)
{
	// テクスチャの開放
	if( g_p3DTextureFade != NULL )
	{
		g_p3DTextureFade -> Release();
	}
	// 頂点バッファの開放
	if( g_pD3DVtxBuffFade != NULL )
	{
		g_pD3DVtxBuffFade -> Release();
	}
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

void UpdateFade(void)
{
    switch( g_fade )
    {
    case FADE_NONE:
        g_color.a = 0.0f;
        break;
    case FADE_IN:
    case BLACK_IN:
		g_color.a -= FADE_RATE;		// α値を減算して画面を浮き上がらせる
		if( g_color.a <= 0.0f )
		{
			// フェード処理終了
			g_color.a = 0.0f;
			g_fade = FADE_NONE;
		}
        break;
    case FADE_OUT:
    case BLACK_OUT:
		g_color.a += FADE_RATE;		// α値を加算して画面を消していく
		if( g_color.a >= 1.0f )
		{
			// フェードイン処理に切り替え
			g_color.a = 1.0f;
			g_fade = g_fadeNext;

			// モードを設定
			SetMode( g_nextGameMode );
		}
        break;
    }
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

void DrawFade(void)
{
	if(g_fade != FADE_NONE)
	{
		// DirectXデバイスの値を取得
		LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

		switch(g_fade)
		{
			case BLACK_IN:
			case BLACK_OUT:
				D3DDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT );
				break;
		}

		// 頂点バッファをデバイスのデータストリームにバインド
		D3DDevice -> SetStreamSource(0, g_pD3DVtxBuffFade, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		D3DDevice -> SetFVF( FVF_VERTEX_2D );

		// テクスチャの設定
		D3DDevice -> SetTexture(0, NULL);

		// ポリゴンの描画
		D3DDevice -> DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
		switch(g_fade)
		{
			case BLACK_IN:
			case BLACK_OUT:
        		D3DDevice -> SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
				break;
		}
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

HRESULT MakeVertexFade(void)
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// オブジェクトの頂点バッファを生成
    if( FAILED( D3DDevice -> CreateVertexBuffer( sizeof(VERTEX_2D) * 4,		// 頂点データ用に確保するバッファサイズ(バイト単位)
												 D3DUSAGE_WRITEONLY,		// 頂点バッファの使用法　
												 FVF_VERTEX_2D,				// 使用する頂点フォーマット
												 D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
												 &g_pD3DVtxBuffFade,		// 頂点バッファインターフェースへのポインタ
												 NULL)))					// NULLに設定
	{
        return E_FAIL;
	}

	//頂点バッファの中身を埋める
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFade -> Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].vtx = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	pVtx[1].vtx = D3DXVECTOR3( SCREEN_WIDTH, 0.0f, 0.0f );
	pVtx[2].vtx = D3DXVECTOR3( 0.0f, SCREEN_HEIGHT, 0.0f );
	pVtx[3].vtx = D3DXVECTOR3( SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f );

	// テクスチャのパースペクティブコレクト用
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = 1.0f;

	// 反射光の設定
	g_color = D3DXCOLOR( 0.f, 0.f, 0.f, 0.01f );

	pVtx[0].diffuse = g_color;
	pVtx[1].diffuse = g_color;
	pVtx[2].diffuse = g_color;
	pVtx[3].diffuse = g_color;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2( 0.0f, 0.0f );
	pVtx[1].tex = D3DXVECTOR2( 1.0f, 0.0f );
	pVtx[2].tex = D3DXVECTOR2( 0.0f, 1.0f );
	pVtx[3].tex = D3DXVECTOR2( 1.0f, 1.0f );	

	// 頂点データをアンロックする
	g_pD3DVtxBuffFade -> Unlock();

	return S_OK;
}

//============================================================================
//
// 色を設定 関数
// 引数：
//　D3DCOLOR col
// 戻り値：
//　なし
//
//============================================================================

void SetColor(D3DCOLOR col)
{
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pD3DVtxBuffFade->Lock( 0, 0, (void**)&pVtx, 0 );

	// 反射光の設定
	pVtx[0].diffuse = col;
	pVtx[1].diffuse = col;
	pVtx[2].diffuse = col;
	pVtx[3].diffuse = col;

	// 頂点データをアンロックする
	g_pD3DVtxBuffFade -> Unlock();
}

//============================================================================
//
// フェードの状態設定 関数
// 引数：
//　FADE out
//  FADE in
//  GAMEMODE next
// 戻り値：
//　なし
//
//============================================================================

void SetFade( FADE out, FADE in, GAMEMODE next )
{
	g_fade         = out;
	g_fadeNext     = in;
	g_nextGameMode = next;
}

//============================================================================
//
// フェードの状態取得 関数
// 引数：
//　なし
// 戻り値：
//　g_fade
//
//============================================================================

FADE GetFade(void)
{
	return g_fade;
}