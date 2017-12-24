//===============================================================================================================
//
// explosion.cpp
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
#include "camera.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"

//============================================================================
//
// マクロ定義
//
//============================================================================

#define MAX_TEX_PATTERN    ( TEX_PATTERN_X * TEX_PATTERN_Y )
#define ANIME_INTERVAL     ( 5 )      // frame

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexExplosion( void );		// 爆発の頂点情報の作成

//============================================================================
//
// グローバル変数
//
//============================================================================

EXPLOSION g_aExplosion[ EXPLOSION_NUM ];

static int g_nAnimeNo;
static int g_nAnimeTime;

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	爆発エフェクトの設定
//	 S_OK	: 爆発エフェクトの設定に成功
//	 E_FAIL	: 爆発エフェクトの設定に失敗
//
//============================================================================

HRESULT InitExplosion( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	g_nAnimeNo   = 0;
	g_nAnimeTime = 0;

	// 爆発エフェクトの位置（向き、大きさ）初期化
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// 爆発エフェクトの位置
		g_aExplosion[ nLoop ].pos = D3DXVECTOR3( 0.0f, BILLBOARD_SIZE + 30.0f, 0.0f );
		
		// 爆発エフェクトの向き
		g_aExplosion[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		
		// 爆発エフェクトの大きさ
		g_aExplosion[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		g_aExplosion[ nLoop ].bUse = false;

	}

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice, 
									EXPLOSION_FILE,
									&g_aExplosion[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
	}

	// 頂点情報の設定
	hr = MakeVertexExplosion();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// 頂点情報の設定に失敗
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

HRESULT MakeVertexExplosion( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// 頂点バッファの生成
	//------------------------------

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// 頂点バッファの作成
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aExplosion[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 爆発エフェクト用の頂点設定
		g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

		pVtx[0].vtx = D3DXVECTOR3( -EXPLOSION_SIZE,  EXPLOSION_SIZE, 0.f );
		pVtx[1].vtx = D3DXVECTOR3(  EXPLOSION_SIZE,  EXPLOSION_SIZE, 0.f );
		pVtx[2].vtx = D3DXVECTOR3( -EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.f );
		pVtx[3].vtx = D3DXVECTOR3(  EXPLOSION_SIZE, -EXPLOSION_SIZE, 0.f );

		for( int nVtx = 0; nVtx < 4; nVtx++ )
		{
			pVtx[ nVtx ].nor = D3DXVECTOR3( 0, 1, 0 );
			pVtx[ nVtx ].diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		}

		g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
	}

	//------------------------------
	
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

void UpdataExplosion( void )
{
	g_nAnimeTime++;

	if( g_nAnimeTime > ANIME_INTERVAL )
	{
		if( g_nAnimeNo >= MAX_TEX_PATTERN )
		{
			for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
			{
				if( g_aExplosion[ nLoop ].bUse == true )
				{
					g_aExplosion[ nLoop ].bUse = false;
					g_nAnimeNo = 0;
				}
			}
		}

		SetTextureAnime( g_nAnimeNo );
		g_nAnimeNo++;
		g_nAnimeTime = 0;
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

void DrawExplosion( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// カメラの情報を取得
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == true )
		{
			//------------------------------
			// ワールドマトリックスの設定
			//------------------------------

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_aExplosion[ nLoop ].mtxWorld );

			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_aExplosion[ nLoop ].scl.x, g_aExplosion[ nLoop ].scl.y, g_aExplosion[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aExplosion[ nLoop ].mtxWorld, &g_aExplosion[ nLoop ].mtxWorld, &mtxScl );

			// 回転を反映
			// 逆行列を求める
			D3DXMatrixInverse( &g_aExplosion[ nLoop ].mtxWorld,	// ワールドマトリックス
							   NULL,
							   &Camera.mtxView );				// ビューマトリックス

			g_aExplosion[ nLoop ].mtxWorld._41 = 0.0f;
			g_aExplosion[ nLoop ].mtxWorld._42 = 0.0f;
			g_aExplosion[ nLoop ].mtxWorld._43 = 0.0f;

			// 移動を反映
			D3DXMatrixTranslation( &mtxTrans, g_aExplosion[ nLoop ].pos.x, g_aExplosion[ nLoop ].pos.y, g_aExplosion[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aExplosion[ nLoop ].mtxWorld, &g_aExplosion[ nLoop ].mtxWorld, &mtxTrans );

			// ワールドマトリックスの設定
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aExplosion[ nLoop ].mtxWorld );


			//--------------------
			// ポリゴン描画処理
			//--------------------

			// 頂点バッファをデータストリームにバインド
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// 頂点フォーマット
			D3DDevice -> SetStreamSource( 0,
										  g_aExplosion[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// テクスチャの設定
			D3DDevice -> SetTexture( 0, g_aExplosion[ 0 ].pD3DTexture );

			// ポリゴンの描画
			//--------------------

			D3DDevice ->SetRenderState(D3DRS_ZENABLE, false);
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// アルファテストを有効にする
			D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// 比較するアルファ値（ 0～255 ）
			D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// 比較する基準

			
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			D3DDevice ->SetRenderState(D3DRS_ZENABLE, true);
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// アルファテストを無効にする

			//--------------------
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

void UnInitExplosion( void )
{
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		// フィールドのバーテックスバッファの終了処理
		if( g_aExplosion[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// フィールドのテクスチャの終了処理
		if( g_aExplosion[ nLoop ].pD3DTexture != NULL )
		{
			g_aExplosion[ nLoop ].pD3DTexture -> Release();
			g_aExplosion[ nLoop ].pD3DTexture = NULL;
		}
	}
}

//============================================================================
//
// テクスチャ座標を、アニメ番号によって決定する処理 関数
// 引数：
//　int animeNo
// 戻り値：
//　なし
//
//============================================================================

//----------------------------------------------------------------
//  テクスチャ座標を、アニメ番号によって決定
//        animeNo   0 ～ (　MAX_TEX_PATTERN - 1　)

void SetTextureAnime( int animeNo )
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	int animeNoX, animeNoY;
	float top, bottom, left, right;

	// テクスチャー全体の縦横何番目のテクスチャか
	animeNoX = animeNo % TEX_PATTERN_X;
	animeNoY = animeNo / TEX_PATTERN_X;

	// テクスチャ座標の値に変換
	top    = ( 1.0f * ( animeNoY )     ) / TEX_PATTERN_Y;
	bottom = ( 1.0f * ( animeNoY + 1 ) ) / TEX_PATTERN_Y;
	left   = ( 1.0f * ( animeNoX )     ) / TEX_PATTERN_X;
	right  = ( 1.0f * ( animeNoX + 1 ) ) / TEX_PATTERN_X;

	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == true )
		{
			// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

			// テクスチャー座標の設定
			pVtx[0].tex = D3DXVECTOR2( left, top );

			pVtx[1].tex = D3DXVECTOR2( right, top );

			pVtx[2].tex = D3DXVECTOR2( left, bottom );

			pVtx[3].tex = D3DXVECTOR2( right, bottom );

			// 頂点データをアンロックする
			g_aExplosion[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
		}
	}
}

//============================================================================
//
// 爆発エフェクト情報取得 関数
// 引数：
//　なし
// 戻り値：
//　g_aExplosion
//
//============================================================================

EXPLOSION* GetExplosion( void )
{
	return g_aExplosion;
}

//============================================================================
//
// 爆発エフェクト情報取得 関数
// 引数：
//　D3DXVECTOR3 pos
// 戻り値：
//　なし
//
//============================================================================

void SetExplosion( D3DXVECTOR3 pos )
{
	for( int nLoop = 0; nLoop < EXPLOSION_NUM; nLoop++ )
	{
		if( g_aExplosion[ nLoop ].bUse == false )
		{
			g_aExplosion[ nLoop ].pos.x = pos.x;
			g_aExplosion[ nLoop ].pos.z = pos.z;
			g_aExplosion[ nLoop ].bUse = true;
			break;
		}
	}
}

/* EOF */