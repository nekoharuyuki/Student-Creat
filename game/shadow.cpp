//===============================================================================================================
//
// shadow.cpp
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
#include "shadow.h"
#include "model.h"
#include "billboard.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

SHADOW g_aShadow[ MAX_SHADOW ];

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

HRESULT InitShadow( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// ビルボードの影の位置（向き、大きさ）初期化
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		// 影の位置
		g_aShadow[ nLoop ].pos = D3DXVECTOR3( 0.0f, 0.05f, 0.0f );

		// 影の向き
		g_aShadow[ nLoop ].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

		// 影の大きさ
		g_aShadow[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

		g_aShadow[ nLoop ].bUse = false;
	
		// テクスチャの読み込み
		hr = D3DXCreateTextureFromFile( D3DDevice, SHADOW_FILE, &g_aShadow[ nLoop ].pD3DTexture );
	}

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
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

void UpdataShadow( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	//------------------------------
	// 頂点バッファの生成
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[nLoop].bUse == true )
		{
			// 頂点バッファの作成
			D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											 D3DUSAGE_WRITEONLY,
											 FVF_VERTEX_3D,
											 D3DPOOL_MANAGED,
											 &g_aShadow[ nLoop ].pD3DVtxBtxBuffer,
											 NULL );

			// 頂点情報の設定
			VERTEX_3D *pVtx;

			// 弾用の影
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

			pVtx[0].vtx = D3DXVECTOR3( -g_aShadow[ nLoop ].fsize, 0.f,  g_aShadow[ nLoop ].fsize );
			pVtx[1].vtx = D3DXVECTOR3(  g_aShadow[ nLoop ].fsize, 0.f,  g_aShadow[ nLoop ].fsize );
			pVtx[2].vtx = D3DXVECTOR3( -g_aShadow[ nLoop ].fsize, 0.f, -g_aShadow[ nLoop ].fsize );
			pVtx[3].vtx = D3DXVECTOR3(  g_aShadow[ nLoop ].fsize, 0.f, -g_aShadow[ nLoop ].fsize );

			for( int nBillboardVtx = 0; nBillboardVtx < 4; nBillboardVtx++ )
			{
				pVtx[ nBillboardVtx ].nor = D3DXVECTOR3( 0, 1, 0 );
				pVtx[ nBillboardVtx ].diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
			}

			pVtx[0].tex = D3DXVECTOR2( 0, 0 );
			pVtx[1].tex = D3DXVECTOR2( 1, 0 );
			pVtx[2].tex = D3DXVECTOR2( 0, 1 );
			pVtx[3].tex = D3DXVECTOR2( 1, 1 );

			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Unlock();
		}
	}

	//------------------------------
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

void DrawShadow( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	//------------------------------
	// ワールドマトリックスの設定
	//------------------------------

	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[ nLoop ].bUse == true )
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_aShadow[ nLoop ].mtxWorld );

			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_aShadow[ nLoop ].scl.x, g_aShadow[ nLoop ].scl.y, g_aShadow[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxScl );

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aShadow[ nLoop ].rot.y, g_aShadow[ nLoop ].rot.x, g_aShadow[ nLoop ].rot.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxRot );

			// 移動を反映
			D3DXMatrixTranslation( &mtxTrans, g_aShadow[ nLoop ].pos.x, g_aShadow[ nLoop ].pos.y, g_aShadow[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aShadow[ nLoop ].mtxWorld, &g_aShadow[ nLoop ].mtxWorld, &mtxTrans );

			// ワールドマトリックスの設定
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aShadow[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// ポリゴン描画処理
			//--------------------

			// 頂点バッファをデータストリームにバインド
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// 頂点フォーマット
			D3DDevice -> SetStreamSource( 0,
										  g_aShadow[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// テクスチャの設定
			D3DDevice -> SetTexture( 0, g_aShadow[ nLoop ].pD3DTexture );

			// 影の描画設定
			//--------------------

			D3DDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			D3DDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			D3DDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			// 描画設定を元に戻す
			//--------------------

			D3DDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			D3DDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			D3DDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

void UnInitShadow( void )
{
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		// フィールドのバーテックスバッファの終了処理
		if( g_aShadow[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aShadow[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// フィールドのテクスチャの終了処理
		if( g_aShadow[ nLoop ].pD3DTexture != NULL )
		{
			g_aShadow[ nLoop ].pD3DTexture -> Release();
			g_aShadow[ nLoop ].pD3DTexture = NULL;
		}
	}
}

int SetShadow( D3DXVECTOR3 pos, float size )
{
	for( int nLoop = 0; nLoop < MAX_SHADOW; nLoop++ )
	{
		if( g_aShadow[ nLoop ].bUse == false )
		{
			g_aShadow[ nLoop ].bUse = true;
			g_aShadow[ nLoop ].fsize = size;
			g_aShadow[ nLoop ].pos.x = pos.x;
			g_aShadow[ nLoop ].pos.z = pos.z;

			return nLoop;
		}
	}
	return -1;
}

void ResetShadow( int nNumber, D3DXVECTOR3 pos )
{
	g_aShadow[ nNumber ].pos.x = pos.x;
	g_aShadow[ nNumber ].pos.z = pos.z;
}

void DestroyShadow( int nNumber )
{
	g_aShadow[ nNumber ].bUse = false;
}

/* EOF */