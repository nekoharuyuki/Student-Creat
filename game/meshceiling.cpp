//===============================================================================================================
//
// mashceiling.cpp
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
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"
#include "dungeon.h"

//============================================================================
//
// マクロ定義
//
//============================================================================

// 頂点数
#define CEILING_VERTEX            ( ( TEXTURE_NUM + 1 ) * ( TEXTURE_NUM + 1 ) )

// インデックス数
#define CEILING_INDEX             ( ( ( CEILING_VERTEX - 2 ) * 2 ) )

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexCeiling( void );		// フィールドの頂点情報の作成

//============================================================================
//
// グローバル変数
//
//============================================================================

static MESH_FIELD g_Ceiling[ MAX_FIELD ];

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	フィールドの設定
//	 S_OK	: フィールドの設定に成功
//	 E_FAIL	: フィールドの設定に失敗
//
//============================================================================

HRESULT InitCeiling( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// 地面の位置（向き、大きさ）初期化
		g_Ceiling[ nLoop ].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// 地面の位置
		g_Ceiling[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// 地面の向き
		g_Ceiling[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// 地面の大きさ

		g_Ceiling[ nLoop ].bUse     = false;
	}

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice, WALL_FILE, &g_Ceiling[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
	}

	// 頂点情報の設定
	hr = MakeVertexCeiling();

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// 頂点情報の設定に失敗
	}

	// ダンジョン作成

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// ダンジョンの情報取得
		int Dungeon = GetDungeon( ( int )( nLoop / FIELD_X ), ( int )( nLoop % FIELD_Z ) );

		// フィールドのポジション
		if( Dungeon == 1 )
		{
			g_Ceiling[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_Ceiling[ nLoop ].pos = D3DXVECTOR3( ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop / FIELD_X ) ),
												  ( float )( ( TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) + 50,
												  ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop % FIELD_Z ) ) );
			g_Ceiling[ nLoop ].bUse = true;
		}
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

HRESULT MakeVertexCeiling( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// 頂点バッファの生成
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// 頂点バッファの作成
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * CEILING_VERTEX,	// 頂点バッファのサイズ
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_Ceiling[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		if( FAILED( hr ) )
		{
			return E_FAIL;	// 頂点バッファの作成に失敗
		}

		// 頂点情報の設定
		VERTEX_3D *pVtx;

		g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, ( void** )&pVtx, 0 );

		// 3Dの頂点座標（ X, Y, Z ）の設定
		//-----------------------------------

		int nCountVtx = 0;

		float fPoint = ( -( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE );
		float fTemp = fPoint;

		for( int nCountVtx_Z = 0; nCountVtx_Z <= TEXTURE_NUM; nCountVtx_Z++ )
		{
			for( int nCountVtx_X = 0; nCountVtx_X <= TEXTURE_NUM; nCountVtx_X++ )
			{
				pVtx[ nCountVtx ].vtx = D3DXVECTOR3( fPoint + ( nCountVtx_X * TEXTURE_SIZE ) , 0.f, -( fTemp ) );
				nCountVtx++;
			}
			fTemp += TEXTURE_SIZE;
		}

		// 法線ベクトル・反射光の設定
		//-----------------------------------

		for( int i = 0; i < CEILING_VERTEX; i++ )
		{
			pVtx[ i ].nor      =  D3DXVECTOR3( 0, 1, 0 );
			pVtx[ i ].diffuse  =  D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		// テクスチャ座標の設定
		//-----------------------------------

		int nCount = 0;

		for( int nTex_Y = 0; nTex_Y <= TEXTURE_NUM; nTex_Y++ )
		{
			for( int nTex_X = 0; nTex_X <= TEXTURE_NUM; nTex_X++ )
			{
				pVtx[nCount].tex = D3DXVECTOR2( ( float ) nTex_X, ( float ) nTex_Y );
				nCount++;
			}
		}

		g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

		//------------------------------

		//------------------------------
		// インデックスバッファの生成
		//------------------------------

		// インデックス情報の作成
		hr = D3DDevice -> CreateIndexBuffer( sizeof( WORD ) * CEILING_INDEX,	// インデックス バッファのサイズ
											 D3DUSAGE_WRITEONLY,
											 D3DFMT_INDEX16,
											 D3DPOOL_MANAGED,
											 &g_Ceiling[ nLoop ].pD3DIndxBuff,
											 NULL);

		if( FAILED( hr ) )
		{
			return E_FAIL;	// インデックスバッファの作成に失敗
		}

		// インデックス情報の設定
		//-----------------------------------

		WORD *pIdx;
		g_Ceiling[ nLoop ].pD3DIndxBuff -> Lock( 0, 0, ( void** )&pIdx, 0 );

		int nOddNumber   = 1;		// 偶数番号の入力用変数
		int nEvenNumber  = 0;		// 奇数番号の入力用変数

		// 縮退ポリゴン計算用変数
		int nDegeneracy      = ( ( TEXTURE_NUM + 1 ) * 2 );
		int nDegeneracy_temp = nDegeneracy + 1;
		int nNext_Degeneracy = nDegeneracy_temp + 1;

		for( int nCountIdx = 0; nCountIdx < CEILING_INDEX; nCountIdx++ )
		{
			// 縮退ポリゴン用の処理
			if( nCountIdx == nDegeneracy )
			{
				pIdx[ nCountIdx ] = nEvenNumber - 1;
				nDegeneracy += nNext_Degeneracy;
			}
			else if( nCountIdx == nDegeneracy_temp )
			{
				pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM;
				nDegeneracy_temp += nNext_Degeneracy;
			}
			else
			{
				if( nCountIdx % 2 == 0 )
				{
					// 偶数番号に入る値
					pIdx[ nCountIdx ] = nOddNumber + TEXTURE_NUM;
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

		g_Ceiling[ nLoop ].pD3DIndxBuff -> Unlock();
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

void UpdataCeiling( void )
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

void DrawCeiling( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		
		if( g_Ceiling[ nLoop ].bUse == true )
		{
			//------------------------------
			// ワールドマトリックスの設定
			//------------------------------
		
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_Ceiling[ nLoop ].mtxWorld );

			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_Ceiling[ nLoop ].scl.x, g_Ceiling[ nLoop ].scl.y, g_Ceiling[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxScl );

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_Ceiling[ nLoop ].rot.y, g_Ceiling[ nLoop ].rot.x, g_Ceiling[ nLoop ].rot.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxRot );

			// 移動を反映
			D3DXMatrixTranslation( &mtxTrans, g_Ceiling[ nLoop ].pos.x, g_Ceiling[ nLoop ].pos.y, g_Ceiling[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_Ceiling[ nLoop ].mtxWorld, &g_Ceiling[ nLoop ].mtxWorld, &mtxTrans );

			// ワールドマトリックスの設定
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_Ceiling[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// ポリゴン描画処理
			//--------------------

			// 頂点バッファをデータストリームにバインド
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			D3DDevice -> SetIndices( g_Ceiling[ nLoop ].pD3DIndxBuff );

			// 頂点フォーマット
			D3DDevice -> SetStreamSource( 0,
										  g_Ceiling[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// テクスチャの設定
			D3DDevice -> SetTexture( 0, g_Ceiling[ 0 ].pD3DTexture );

			D3DDevice -> DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	// レンダリングするプリミティブの種類
											   0,
											   0,
											   CEILING_VERTEX,		// 使用される頂点の数
											   0,
											   CEILING_INDEX - 2 );	// レンダリングするプリミティブの数

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

void UnInitCeiling( void )
{
	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// フィールドのバーテックスバッファの終了処理
		if( g_Ceiling[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_Ceiling[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_Ceiling[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// フィールドのインデックスバッファの終了処理
		if( g_Ceiling[ nLoop ].pD3DIndxBuff != NULL )
		{
			g_Ceiling[ nLoop ].pD3DIndxBuff -> Release();
			g_Ceiling[ nLoop ].pD3DIndxBuff = NULL;
		}
		// フィールドのテクスチャの終了処理
		if( g_Ceiling[ nLoop ].pD3DTexture != NULL )
		{
			g_Ceiling[ nLoop ].pD3DTexture -> Release();
			g_Ceiling[ nLoop ].pD3DTexture = NULL;
		}
	}
}

MESH_FIELD GetCeiling( int nIdx )
{
	return g_Ceiling[ nIdx ];
}