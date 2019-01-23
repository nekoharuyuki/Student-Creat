//===============================================================================================================
//
// explosion.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

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
#define EXPLOSION_FILE              ( "data/TEXTURE/Special2.png" )
#define TEX_PATTERN_X               ( 5 )
#define TEX_PATTERN_Y               ( 3 )

// テクスチャのサイズ
#define EXPLOSION_SIZE              ( 80.f )	// 爆発

// 爆発の数
#define EXPLOSION_NUM               ( 1 + BILLBOARD_NUM )

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

	D3DXMATRIX mtxWorld;		// ワールドマトリックス

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scl;		// 大きさ

	int nAnimeNo;
	int nAnimeTime;

	bool bUse;
}EXPLOSION;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitExplosion(void);	// 爆発エフェクトの初期化 処理
void DrawExplosion(void);		// 爆発エフェクトの描画 処理
void UpdataExplosion(void);		// 爆発エフェクトの更新 処理
void UnInitExplosion(void);		// 爆発エフェクトの開放 処理

void SetTextureAnime(int animeNo);

EXPLOSION* GetExplosion(void);	// 爆発エフェクトの情報取得 処理

void SetExplosion( D3DXVECTOR3 pos );

/* インクルードガードの最終行 */
#endif	// _EXPLOSION_H_