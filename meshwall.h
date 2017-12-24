//===============================================================================================================
//
// mashfield.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _WALL_H_	// インクルードガード開始
#define _WALL_H_	// インクルードガード用マクロ定義

//============================================================================
//
// マクロ定義
//
//============================================================================

// 地面のテクスチャファイル
#define WALL_FILE              ( "data/TEXTURE/wall000.jpg" )
#define WALL_SKY               ( "data/TEXTURE/sky001.jpg" )

#define MAX_WALL               ( 4 )
#define MAX_SKY                ( 3 )

//============================================================================
//
// 構造体定義
//
//============================================================================

/* 壁用 構造体 */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// バーテックスバッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  pD3DIndxBuff;		// インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9      pD3DTexture;		// テクスチャ

	D3DXMATRIX mtxWorld;		// ワールドマトリックス

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scl;		// 大きさ

	int nNumBlockX, nNumBlockY;
	int nNumVertex;
	int nNumIndex;
	int nNumPolygon;
	float fSizeBlockX, fSizeBlockY;
	bool bUse;
}MESH_WALL;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitWall(void);		// ウォールの初期化
void DrawWall(void);		// ウォールの描画処理
void UpdataWall(void);		// ウォールの更新処理
void UnInitWall(void);		// ウォールの開放処理

// ウォール設定用
HRESULT InitWallUp(void);
void UpdataWallUp(void);
void DrawWallUp(void);
void UnInitWallUp(void);

HRESULT InitWallDown(void);
void UpdataWallDown(void);
void DrawWallDown(void);
void UnInitWallDown(void);

HRESULT InitWallLeft(void);
void UpdataWallLeft(void);
void DrawWallLeft(void);
void UnInitWallLeft(void);

HRESULT InitWallRight(void);
void UpdataWallRight(void);
void DrawWallRight(void);
void UnInitWallRight(void);

// 世界の設定用
HRESULT InitSky(void);
void UpdataSky(void);
void DrawSky(void);
void UnInitSky(void);

/* インクルードガードの最終行 */
#endif	// _WALL_H_