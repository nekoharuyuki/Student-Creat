//===============================================================================================================
//
// billboard.cpp
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
#include "billboard.h"
#include "explosion.h"
#include "camera.h"
#include "model.h"
#include "meshfield.h"
#include "shadow.h"
#include "dungeon.h"

/* サウンドに関するヘッダーファイル */
#include "com.h"
#include "sound.h"
#include "snddata.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexBillboard( void );		// 影の頂点情報の作成

//============================================================================
//
// グローバル変数
//
//============================================================================

static BILLBOARD g_aBillboard[ BILLBOARD_NUM ];

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

HRESULT InitBillboard( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		// 影の位置（向き、大きさ）初期化
		g_aBillboard[ nLoop ].pos = D3DXVECTOR3( 0.0f, BILLBOARD_SIZE + 30.0f, 0.0f );	// 位置
		g_aBillboard[ nLoop ].rot = D3DXVECTOR3( 0.0f, D3DXToRadian( 90 ), 0.0f );		// 向き
		g_aBillboard[ nLoop ].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );					// 大きさ

		g_aBillboard[ nLoop ].bUse = false;

		g_aBillboard[ nLoop ].fShadowSize = SHADOW_SIZE_BILLBOARD;
	}

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice, BILLBOARD_FILE, &g_aBillboard[ 0 ].pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
	}

	// 頂点情報の設定
	hr = MakeVertexBillboard();

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

HRESULT MakeVertexBillboard( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		//------------------------------
		// 頂点バッファの生成
		//------------------------------

		// 頂点バッファの作成
		hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
											  D3DUSAGE_WRITEONLY,
											  FVF_VERTEX_3D,
											  D3DPOOL_MANAGED,
											  &g_aBillboard[ nLoop ].pD3DVtxBtxBuffer,
											  NULL );

		// 頂点情報の設定
		VERTEX_3D *pVtx;

		g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

		pVtx[0].vtx = D3DXVECTOR3( -BILLBOARD_SIZE,  BILLBOARD_SIZE, 0.0f );
		pVtx[1].vtx = D3DXVECTOR3(  BILLBOARD_SIZE,  BILLBOARD_SIZE, 0.0f );
		pVtx[2].vtx = D3DXVECTOR3( -BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f );
		pVtx[3].vtx = D3DXVECTOR3(  BILLBOARD_SIZE, -BILLBOARD_SIZE, 0.0f );

		for( int i = 0; i < 4; i++ )
		{
			pVtx[ i ].nor = D3DXVECTOR3( 0, 1, 0 );
			pVtx[ i ].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
		}

		pVtx[0].tex = D3DXVECTOR2( 0, 0 );
		pVtx[1].tex = D3DXVECTOR2( 1, 0 );
		pVtx[2].tex = D3DXVECTOR2( 0, 1 );
		pVtx[3].tex = D3DXVECTOR2( 1, 1 );

		g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Unlock();

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

void UpdataBillboard( void )
{
	//-------------------------
	// 弾の当たり判定処理
	//-------------------------

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if( g_aBillboard[ nLoop ].bUse == true )
		{
			// 移動処理
			g_aBillboard[ nLoop ].pos.x += -sin( g_aBillboard[ nLoop ].Move.y ) * BILLBOARD_MOVE_SPEED;
			g_aBillboard[ nLoop ].pos.z += -cos( g_aBillboard[ nLoop ].Move.y ) * BILLBOARD_MOVE_SPEED;

			// 影の処理
			ResetShadow(g_aBillboard[ nLoop ].nShadowNumber, g_aBillboard[ nLoop ].pos);

			if( g_aBillboard[ nLoop ].pos.z < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) )        ||
				g_aBillboard[ nLoop ].pos.z > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) - 100.f ||
				g_aBillboard[ nLoop ].pos.x < ( -( ( (float)TEXTURE_NUM / 2 ) * TEXTURE_SIZE ) )        ||
				g_aBillboard[ nLoop ].pos.x > ( ( (float)TEXTURE_NUM * WORLD ) * TEXTURE_SIZE ) )
			{
				// サウンド再生
				PlaySnd( SOUND_LABEL_SE002 );
				
				// 爆発を設定
				SetExplosion( g_aBillboard[ nLoop ].pos );
				
				// 弾の消去
				g_aBillboard[ nLoop ].bUse = false;
				
				// 影の消去
				DestroyShadow(g_aBillboard[ nLoop ].nShadowNumber);
				
				break;
			}

			int posX= (int)( g_aBillboard[ nLoop ].pos.x + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );
			int posZ= (int)( g_aBillboard[ nLoop ].pos.z + (float)( ( TEXTURE_NUM * TEXTURE_SIZE ) / 2 ) ) / ( TEXTURE_NUM * TEXTURE_SIZE );

			int Dungeon = GetDungeon( ( int )( posX ), ( int )( posZ ) );

			if( Dungeon == 1 )
			{
				// サウンド再生
				PlaySnd( SOUND_LABEL_SE002 );

				// 爆発を設定
				SetExplosion( g_aBillboard[ nLoop ].pos );

				// 弾の消去
				g_aBillboard[ nLoop ].bUse = false;

				// 影の消去
				DestroyShadow(g_aBillboard[ nLoop ].nShadowNumber);
				break;
			}
		}
	}

	//-------------------------

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

void DrawBillboard( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// カメラの情報を取得
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if(g_aBillboard[ nLoop ].bUse == true)
		{
			//------------------------------
			// ワールドマトリックスの設定
			//------------------------------

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity( &g_aBillboard[ nLoop ].mtxWorld );

			// スケールを反映
			D3DXMatrixScaling( &mtxScl, g_aBillboard[ nLoop ].scl.x, g_aBillboard[ nLoop ].scl.y, g_aBillboard[ nLoop ].scl.z );
			D3DXMatrixMultiply( &g_aBillboard[ nLoop ].mtxWorld, &g_aBillboard[ nLoop ].mtxWorld, &mtxScl );

			// 逆行列を求める
			D3DXMatrixInverse( &g_aBillboard[ nLoop ].mtxWorld,	// ワールドマトリックス
							   NULL,
							   &Camera.mtxView );				// ビューマトリックス

			g_aBillboard[ nLoop ].mtxWorld._41 = 0.0f;
			g_aBillboard[ nLoop ].mtxWorld._42 = 0.0f;
			g_aBillboard[ nLoop ].mtxWorld._43 = 0.0f;

			// 移動を反映
			D3DXMatrixTranslation( &mtxTrans, g_aBillboard[ nLoop ].pos.x, g_aBillboard[ nLoop ].pos.y, g_aBillboard[ nLoop ].pos.z );
			D3DXMatrixMultiply( &g_aBillboard[ nLoop ].mtxWorld, &g_aBillboard[ nLoop ].mtxWorld, &mtxTrans );

			// ワールドマトリックスの設定
			D3DDevice -> SetTransform( D3DTS_WORLD, &g_aBillboard[ nLoop ].mtxWorld );

			//------------------------------

			//--------------------
			// ポリゴン描画処理
			//--------------------

			// 頂点バッファをデータストリームにバインド
			D3DDevice -> SetFVF( FVF_VERTEX_3D );

			// 頂点フォーマット
			D3DDevice -> SetStreamSource( 0,
										  g_aBillboard[ nLoop ].pD3DVtxBtxBuffer,
										  0,
										  sizeof( VERTEX_3D ) );

			// テクスチャの設定
			D3DDevice -> SetTexture( 0, g_aBillboard[ 0 ].pD3DTexture );

			// アルファテスト
			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// アルファテストを有効にする
			D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// 比較するアルファ値（ 0～255 ）
			D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// 比較する基準

			
			D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
										0,
										2 );

			//--------------------

			D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// アルファテストを無効にする
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

void UnInitBillboard( void )
{
	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		// ビルボードのバーテックスバッファの終了処理
		if( g_aBillboard[ nLoop ].pD3DVtxBtxBuffer != NULL )
		{
			g_aBillboard[ nLoop ].pD3DVtxBtxBuffer -> Release();
			g_aBillboard[ nLoop ].pD3DVtxBtxBuffer = NULL;
		}
		// ビルボードのテクスチャの終了処理
		if( g_aBillboard[ nLoop ].pD3DTexture != NULL )
		{
			g_aBillboard[ nLoop ].pD3DTexture -> Release();
			g_aBillboard[ nLoop ].pD3DTexture = NULL;
		}
	}
}

//============================================================================
//
// ビルボードの情報取得 関数
// 引数：
//　なし
// 戻り値：
//　g_aBillboard
//
//============================================================================

BILLBOARD* GetBillboard( void )
{
	return g_aBillboard;
}

void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot )
{
	for( int nLoop = 0; nLoop < BILLBOARD_NUM; nLoop++ )
	{
		if( g_aBillboard[ nLoop ].bUse == false )
		{
			// サウンド再生


			// 各種設定
			g_aBillboard[ nLoop ].pos.x  = pos.x;
			g_aBillboard[ nLoop ].pos.y  = pos.y + 30.0f;
			g_aBillboard[ nLoop ].pos.z  = pos.z;
			g_aBillboard[ nLoop ].Move.y = rot.y;
			g_aBillboard[ nLoop ].nShadowNumber = SetShadow( g_aBillboard[ nLoop ].pos, g_aBillboard[ nLoop ].fShadowSize );
			g_aBillboard[ nLoop ].bUse   = true;
			break;
		}
	}
}

/* EOF */