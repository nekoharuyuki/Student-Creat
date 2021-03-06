//===============================================================================================================
//
// field.cpp
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
#include "light.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

static D3DLIGHT9 g_aLight[ 3 ];		// ライト情報

//============================================================================
//
// ライトの初期化 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

/* Direct3D で利用できるライトには、次の 3 種類がある。 */

//	ポイント ライト				…　D3DLIGHT_POINT			1
//	ディレクショナル ライト		…　D3DLIGHT_SPOT			2
//	スポットライト				…　D3DLIGHT_DIRECTIONAL	3

void InitLight( void )
{
	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// ライトの向き
	D3DXVECTOR3 vecDir[ 3 ];

	// ライトの作成(強)
	ZeroMemory( &g_aLight[ 0 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 0 ].Type = D3DLIGHT_DIRECTIONAL;					// ライトの種類
	g_aLight[ 0 ].Diffuse = D3DXCOLOR( 0.9f, 0.8f, 0.8f, 1.f );	// 拡散光設定 ( R, G, B, A )
	vecDir[ 0 ] = D3DXVECTOR3( -0.5f, -0.8f, 0.3f );			// ライトの方向

	// ライトの作成(中)
	ZeroMemory( &g_aLight[ 1 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 1 ].Type = D3DLIGHT_DIRECTIONAL;					// ライトの種類
	g_aLight[ 1 ].Diffuse = D3DXCOLOR( 0.7f, 0.5f, 0.4f, 1.f );	// 拡散光設定 ( R, G, B, A )
	vecDir[ 1 ] = D3DXVECTOR3( 0.2f, 0.5f, 0.5f );				// ライトの方向

	// ライトの作成(弱)
	ZeroMemory( &g_aLight[ 2 ], sizeof( D3DLIGHT9 ) );
	g_aLight[ 2 ].Type = D3DLIGHT_DIRECTIONAL;					// ライトの種類
	g_aLight[ 2 ].Diffuse = D3DXCOLOR( 0.5f, 0.3f, 0.2f, 1.f );	// 拡散光設定 ( R, G, B, A )
	vecDir[ 2 ] = D3DXVECTOR3( 0.3f, -0.5f, -0.5f );			// ライトの方向

	// ライトを設定
	for( int nLoop = 0; nLoop < 3; nLoop++ )
	{

		D3DXVec3Normalize( ( D3DXVECTOR3* ) &g_aLight[ nLoop ].Direction, &vecDir[ nLoop ] );
		D3DDevice -> SetLight( nLoop, &g_aLight[ nLoop ] );
		D3DDevice -> LightEnable( nLoop, TRUE );
	}

	D3DDevice -> SetRenderState( D3DRS_LIGHTING, TRUE );
}

//============================================================================
//
// ライトの更新 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UpdataLight( void )
{
	;
}

//============================================================================
//
// ライトの描画 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void DrawLight( void )
{
	;
}

//============================================================================
//
// ライトの終了処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UnInitLight( void )
{
	;
}

/* EOF */