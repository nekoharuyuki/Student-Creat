//===============================================================================================================
//
// goal.cpp
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
#include "camera.h"
#include "meshfield.h"
#include "shadow.h"
#include "goal.h"
#include "dungeon.h"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT MakeVertexGoal( void );		// 頂点情報の作成

//============================================================================
//
// グローバル変数
//
//============================================================================

static BILLBOARD g_Goal;

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

HRESULT InitGoal( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	// 影の位置（向き、大きさ）初期化
	g_Goal.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );		// 向き
	g_Goal.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );		// 大きさ

	g_Goal.fShadowSize = SHADOW_SIZE_GOAL;

	for( int nLoop = 0; nLoop < MAX_FIELD; nLoop++ )
	{
		// ダンジョンの情報取得
		int Dungeon = GetDungeon( ( int )( nLoop / FIELD_X ), ( int )( nLoop % FIELD_Z ) );

		// フィールドのポジション
		if( Dungeon == 2 )
		{
			g_Goal.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
			g_Goal.pos = D3DXVECTOR3( ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop / FIELD_X ) ),
									  GOAL_SIZE / 2,
									  ( float )( TEXTURE_NUM * TEXTURE_SIZE * ( int )( nLoop % FIELD_Z ) ) );
			g_Goal.bUse = true;
		}
	}

	// テクスチャの読み込み
	hr = D3DXCreateTextureFromFile( D3DDevice, GOAL_FILE, &g_Goal.pD3DTexture );

	if( FAILED( hr ) ) 
	{
		return E_FAIL;	// テクスチャの読み込みに失敗
	}

	// 頂点情報の設定
	hr = MakeVertexGoal();

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

HRESULT MakeVertexGoal( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	HRESULT hr;

	//------------------------------
	// 頂点バッファの生成
	//------------------------------

	// 頂点バッファの作成
	hr = D3DDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * 4, 
										  D3DUSAGE_WRITEONLY,
										  FVF_VERTEX_3D,
										  D3DPOOL_MANAGED,
										  &g_Goal.pD3DVtxBtxBuffer,
										  NULL );

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	g_Goal.pD3DVtxBtxBuffer -> Lock( 0, 0, (void**)&pVtx, 0 );

	pVtx[0].vtx = D3DXVECTOR3( -GOAL_SIZE,  GOAL_SIZE, 0.0f );
	pVtx[1].vtx = D3DXVECTOR3(  GOAL_SIZE,  GOAL_SIZE, 0.0f );
	pVtx[2].vtx = D3DXVECTOR3( -GOAL_SIZE, -GOAL_SIZE, 0.0f );
	pVtx[3].vtx = D3DXVECTOR3(  GOAL_SIZE, -GOAL_SIZE, 0.0f );

	for( int i = 0; i < 4; i++ )
	{
		pVtx[ i ].nor = D3DXVECTOR3( 0, 1, 0 );
		pVtx[ i ].diffuse = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	}

	pVtx[0].tex = D3DXVECTOR2( 0, 0 );
	pVtx[1].tex = D3DXVECTOR2( 1, 0 );
	pVtx[2].tex = D3DXVECTOR2( 0, 1 );
	pVtx[3].tex = D3DXVECTOR2( 1, 1 );

	g_Goal.pD3DVtxBtxBuffer -> Unlock();

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

void UpdataGoal( void )
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

void DrawGoal( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// カメラの情報を取得
	CAMERA Camera = GetCamera();

	D3DXMATRIX mtxScl, mtxRot, mtxTrans;

	if(g_Goal.bUse == true)
	{
		//------------------------------
		// ワールドマトリックスの設定
		//------------------------------

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity( &g_Goal.mtxWorld );

		// スケールを反映
		D3DXMatrixScaling( &mtxScl, g_Goal.scl.x, g_Goal.scl.y, g_Goal.scl.z );
		D3DXMatrixMultiply( &g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxScl );

		// 逆行列を求める
		D3DXMatrixInverse( &g_Goal.mtxWorld,	// ワールドマトリックス
						   NULL,
						   &Camera.mtxView );	// ビューマトリックス

		g_Goal.mtxWorld._41 = 0.0f;
		g_Goal.mtxWorld._42 = 0.0f;
		g_Goal.mtxWorld._43 = 0.0f;

		// 移動を反映
		D3DXMatrixTranslation( &mtxTrans, g_Goal.pos.x, g_Goal.pos.y, g_Goal.pos.z );
		D3DXMatrixMultiply( &g_Goal.mtxWorld, &g_Goal.mtxWorld, &mtxTrans );

		// ワールドマトリックスの設定
		D3DDevice -> SetTransform( D3DTS_WORLD, &g_Goal.mtxWorld );

		//------------------------------

		//--------------------
		// ポリゴン描画処理
		//--------------------

		// 頂点バッファをデータストリームにバインド
		D3DDevice -> SetFVF( FVF_VERTEX_3D );

		// 頂点フォーマット
		D3DDevice -> SetStreamSource( 0,
									  g_Goal.pD3DVtxBtxBuffer,
									  0,
									  sizeof( VERTEX_3D ) );

		// テクスチャの設定
		D3DDevice -> SetTexture( 0, g_Goal.pD3DTexture );

		// アルファテスト
		D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, true );				// アルファテストを有効にする
		D3DDevice -> SetRenderState( D3DRS_ALPHAREF, 140 );						// 比較するアルファ値（ 0～255 ）
		D3DDevice -> SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );	// 比較する基準

		
		D3DDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP,
									0,
									2 );

		//--------------------

#ifdef _DEBUG	//デバッグ用コード開始

		// デバッグ情報表示
		PrintDebugProc("GOAL POS  : ( %.2f : %.2f : %.2f )\n", g_Goal.pos.x, g_Goal.pos.y, g_Goal.pos.z);

#endif //_DEBUG	//デバッグ用コード終了

		D3DDevice -> SetRenderState( D3DRS_ALPHATESTENABLE, false );			// アルファテストを無効にする
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

void UnInitGoal( void )
{
	// ビルボードのバーテックスバッファの終了処理
	if( g_Goal.pD3DVtxBtxBuffer != NULL )
	{
		g_Goal.pD3DVtxBtxBuffer -> Release();
		g_Goal.pD3DVtxBtxBuffer = NULL;
	}
	// ビルボードのテクスチャの終了処理
	if( g_Goal.pD3DTexture != NULL )
	{
		g_Goal.pD3DTexture -> Release();
		g_Goal.pD3DTexture = NULL;
	}
}

//============================================================================
//
// ビルボードの情報取得　(ゴール) 処理 関数
// 引数：
//　なし
// 戻り値：
//　g_Goal
//
//============================================================================

BILLBOARD GetGoal(void)
{
	return g_Goal;
}

/* EOF */