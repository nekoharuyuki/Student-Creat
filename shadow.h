//===============================================================================================================
//
// shadow.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _SHADOW_H_
#define _SHADOW_H_

//============================================================================
//
// インクルード
//
//============================================================================

#include "billboard.h"

//============================================================================
//
// マクロ定義
//
//============================================================================

// 地面のテクスチャファイル
#define SHADOW_FILE              ( "data/TEXTURE/shadow000.jpg" )

// テクスチャのサイズ
#define SHADOW_SIZE_MODEL              ( 40.f )	// モデル
#define SHADOW_SIZE_BILLBOARD          ( 8.f )	// ビルボード
#define SHADOW_SIZE_GOAL               ( 50.f )	// ビルボード(ゴール)

// 影の種類
#define MAX_SHADOW                     ( 1 + BILLBOARD_NUM )	// 最大数

//============================================================================
//
// 構造体定義
//
//============================================================================

/* 壁用 構造体 */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// バーテックスバッファへのポインタ
	LPDIRECT3DTEXTURE9      pD3DTexture;		// テクスチャ

	D3DXMATRIX mtxWorld;	// ワールドマトリックス

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scl;		// 大きさ

	bool bUse;
	float fsize;
}SHADOW;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitShadow(void);	// シャドウの初期化
void DrawShadow(void);		// シャドウの描画処理
void UpdataShadow(void);	// シャドウの更新処理
void UnInitShadow(void);	// シャドウの開放処理

int SetShadow(D3DXVECTOR3 pos, float size);
void ResetShadow(int nNumber, D3DXVECTOR3 pos);
void DestroyShadow(int nNumber);

/* インクルードガードの最終行 */
#endif	// _SHADOW_H_