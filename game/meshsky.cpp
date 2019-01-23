//===============================================================================================================
//
// mashwall.cpp
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
#include "meshwall.h"
#include "meshfield.h"
#include "dungeon.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexSky( void );		// 空の頂点情報の作成

//============================================================================
//
// グローバル変数
//
//============================================================================

MESH_WALL g_aMeshSky[ MAX_SKY ];
int g_nNumMeshSky = 0;

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	壁の設定
//	 S_OK	: 壁の設定に成功
//	 E_FAIL	: 壁の設定に失敗
//
//============================================================================

HRESULT InitSky( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	g_nNumMeshSky = 0;

	HRESULT hr;

	for( int nLoop = 0; nLoop < MAX_SKY; nLoop++ )
	{
		// 壁用の構造体変数の初期化
		//-----------------------------------

		// 頂点数
		g_aMeshSky[ nLoop ].nNumVertex  = ( ( TEXTURE_NUM * WORLD + 1 ) * ( TEXTURE_NUM * WORLD + 1 ) );

		// インデックス数
		g_aMeshSky[ nLoop ].nNumIndex   = ( ( ( g_aMeshSky[ nLoop ].nNumVertex - 2 ) * 2 ) );

		// ポリゴン数
		g_aMeshSky[ nLoop ].nNumPolygon = TEXTURE_NUM;

		g_aMeshSky[ nLoop ].nNumBlockX  = TEXTURE_SIZE;
		g_aMeshSky[ nLoop ].nNumBlockY  = TEXTURE_SIZE;
		g_aMeshSky[ nLoop ].fSizeBlockX = ( -( (float)g_aMeshSky[ nLoop ].nNumPolygon / 2 ) * g_aMeshSky[ nLoop ].nNumBlockX );
		g_aMeshSky[ nLoop ].fSizeBlockY = ( -( (float)g_aMeshSky[ nLoop ].nNumPolygon / 2 ) * g_aMeshSky[ nLoop ].nNumBlockY );
		g_aMeshSky[ nLoop ].bUse        = false;

		// 壁の位置（向き、大きさ）初期化
		//-----------------------------------

		// 壁の位置・向き
		// 上
		g_aMeshSky[ 0 ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		g_aMeshSky[ 0 ].pos = D3DXVECTOR3( 0.0f, ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 150.f );

		// 右
		g_aMeshSky[ 1 ].rot = D3DXVECTOR3( 0.0f, ( D3DX_PI / 2 ), 0.0f );
		g_aMeshSky[ 1 ].pos = D3DXVECTOR3( ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 150.f, ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 300.f );
		
		// 左
		g_aMeshSky[ 2 ].rot = D3DXVECTOR3( 0.0f, -( D3DX_PI / 2 ), 0.0f );
		g_aMeshSky[ 2 ].pos = D3DXVECTOR3( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ), ( ( (float)TEXTURE_NUM * WORLD / 2 ) * TEXTURE_SIZE ), 0.0f );

		// 壁の大きさ
		g_aMeshSky[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		//-----------------------------------

		// テクスチャの読み込み
		hr = D3DXCreateTextureFromFile( D3DDevice,		// 壁のテクスチャファイル
										WALL_SKY,
										&g_aMeshSky[ nLoop ].pD3DTexture );

		if( FAILED( hr ) ) 
		{
			return E_FAIL;	// テクスチャの読み込みに失敗
		}

		// 壁の数
		g_nNumMeshSky++;
	}

	// 頂点情報の設定
	hr = MakeVertexSky();

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

HRESULT MakeVertexSky( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		//------------------------------
		// 頂点バッファの生成
		//------------------------------

		// 頂点バッファの作成
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * g_aMeshSky[ nLoop ].nNumVertex,	// 頂点バッファのサイズ
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		if( FAILED( hr ) )
		{
			return E_FAIL;	// 頂点バッファの作成に失敗
		}

		// 頂点情報の設定
		VERTEX_3D *pVtx;

		g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, ( void** )&pVtx, 0 );

		// 3Dの頂点座標（ X, Y, Z ）の設定
		//-----------------------------------

		int nCountVtx = 0;
		float fTemp = g_aMeshSky[ nLoop ].fSizeBlockY;

		for( int nCountVtx_Y = 0; nCountVtx_Y <= TEXTURE_NUM * WORLD; nCountVtx_Y++ )
		{
			for( int nCountVtx_X = 0; nCountVtx_X <= TEXTURE_NUM * WORLD; nCountVtx_X++ )
			{
				pVtx[ nCountVtx ].vtx = D3DXVECTOR3( g_aMeshSky[ nLoop ].fSizeBlockX + ( nCountVtx_X * TEXTURE_SIZE ) , -( fTemp ), 0.f );
				nCountVtx++;
			}
			fTemp += TEXTURE_SIZE;
		}

		// 法線ベクトル・反射光の設定
		//-----------------------------------

		for( int nCount = 0; nCount < g_aMeshSky[ nLoop ].nNumVertex; nCount++ )
		{
			pVtx[ nCount ].nor      =  D3DXVECTOR3( 0, 0, -1.0f );
			pVtx[ nCount ].diffuse  =  D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		// テクスチャ座標の設定
		//-----------------------------------

		int nCountTex = 0;

		for( int nTex_Y = 0; nTex_Y <= TEXTURE_NUM * WORLD; nTex_Y++ )
		{
			for( int nTex_X = 0; nTex_X <= TEXTURE_NUM * WORLD; nTex_X++ )
			{
				pVtx[ nCountTex ].tex = D3DXVECTOR2( ( float ) nTex_X, ( float ) nTex_Y );
				nCountTex++;
			}
		}

		g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------

		//------------------------------
		// インデックスバッファの生成
		//------------------------------

		// インデックス情報の作成
		hr = D3DDevice -> CreateIndexBuffer( sizeof( WORD ) * g_aMeshSky[ nLoop ].nNumIndex,	// インデックス バッファのサイズ
											 D3DUSAGE_WRITEONLY,
											 D3DFMT_INDEX16,
											 D3DPOOL_MANAGED,
											 &g_aMeshSky[ nLoop ].pD3DIndxBuff,
											 NULL);

		if( FAILED( hr ) )
		{
			return E_FAIL;	// インデックスバッファの作成に失敗
		}

		// インデックス情報の設定
		//-----------------------------------

		WORD *pIdx;
		g_aMeshSky[ nLoop ].pD3DIndxBuff ->Lock( 0, 0, ( void** )&pIdx, 0 );

		int nOddNumber   = 1;		// 偶数番号の入力用変数
		int nEvenNumber  = 0;		// 奇数番号の入力用変数

		// 縮退ポリゴン計算用変数
		int nDegeneracy = ( ( TEXTURE_NUM * WORLD + 1 ) * 2 );
		int nDegeneracy_temp = nDegeneracy + 1;
		int nNext_Degeneracy = nDegeneracy_temp + 1;

		for( int nCountIdx = 0; nCountIdx < g_aMeshSky[ nLoop ].nNumIndex; nCountIdx++ )
		{
			// 縮退ポリゴン用の処理
			if( nCountIdx == nDegeneracy )
			{
				pIdx[ nCountIdx ] = nEvenNumber - 1;
				nDegeneracy += nNext_Degeneracy;
			}
			else if( nCountIdx == nDegeneracy_temp )
			{
				pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM * WORLD;
				nDegeneracy_temp += nNext_Degeneracy;
			}
			else
			{
				if( nCountIdx % 2 == 0 )
				{
					// 偶数番号に入る値
					pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM * WORLD;
					nOddNumber++;
				}
				if( nCountIdx % 2 == 1 )
				{
					// 奇数番号に入る値
					pIdx[ nCountIdx ] = nEvenNumber;
					nEvenNumber++;
				}
			}	
		}

		g_aMeshSky[ nLoop ].pD3DIndxBuff -> Unlock();

		//------------------------------
	}

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

void UpdataSky( void )
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

void DrawSky( void )
{
	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		// DirectXデバイスの値を取得
		LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

		D3DXMATRIX mtxScl, mtxRot, mtxTrans;

		//------------------------------
		// ワールドマトリックスの設定
		//------------------------------

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &g_aMeshSky[ nLoop ].mtxWorld );

		// スケールを反映
		D3DXMatrixScaling( &mtxScl, g_aMeshSky[ nLoop ].scl.x, g_aMeshSky[ nLoop ].scl.y, g_aMeshSky[ nLoop ].scl.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxScl );

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aMeshSky[ nLoop ].rot.y, g_aMeshSky[ nLoop ].rot.x, g_aMeshSky[ nLoop ].rot.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxRot );

		// 移動を反映
		D3DXMatrixTranslation( &mtxTrans, g_aMeshSky[ nLoop ].pos.x, g_aMeshSky[ nLoop ].pos.y, g_aMeshSky[ nLoop ].pos.z );
		D3DXMatrixMultiply( &g_aMeshSky[ nLoop ].mtxWorld, &g_aMeshSky[ nLoop ].mtxWorld, &mtxTrans );

		// ワールドマトリックスの設定
		D3DDevice -> SetTransform( D3DTS_WORLD, &g_aMeshSky[ nLoop ].mtxWorld );

		//------------------------------

		//--------------------
		// ポリゴン描画処理
		//--------------------

		// 頂点バッファをデータストリームにバインド
		D3DDevice -> SetFVF( FVF_VERTEX_3D );

		D3DDevice -> SetIndices( g_aMeshSky[ nLoop ].pD3DIndxBuff );

		// 頂点フォーマット
		D3DDevice -> SetStreamSource( 0,
									  g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer,
									  0,
									  sizeof( VERTEX_3D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_aMeshSky[ nLoop ].pD3DTexture );

		// ポリゴンの描画
		D3DDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	// レンダリングするプリミティブの種類
										   0,
										   0,
										   g_aMeshSky[ nLoop ].nNumVertex,			// 使用される頂点の数
										   0,
										   g_aMeshSky[ nLoop ].nNumIndex - 2 );	// レンダリングするプリミティブの数

		//--------------------
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

void UnInitSky( void )
{
	for( int nLoop = 0; nLoop < g_nNumMeshSky; nLoop++ )
	{
		// フィールドのバーテックスバッファの終了処理
		if( g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aMeshSky[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// フィールドのインデックスバッファの終了処理
		if( g_aMeshSky[ nLoop ].pD3DIndxBuff != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DIndxBuff -> Release();
			g_aMeshSky[ nLoop ].pD3DIndxBuff = NULL;
		}
		// フィールドのテクスチャの終了処理
		if( g_aMeshSky[ nLoop ].pD3DTexture != NULL )
		{
			g_aMeshSky[ nLoop ].pD3DTexture -> Release();
			g_aMeshSky[ nLoop ].pD3DTexture = NULL;
		}
	}
}

/* EOF */