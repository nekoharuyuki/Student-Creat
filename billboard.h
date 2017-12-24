//===============================================================================================================
//
// billboard.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// ビルボードのテクスチャファイル
#define BILLBOARD_FILE              ( "data/TEXTURE/bullet000.png" )

#define BILLBOARD_MOVE_SPEED        ( 10.f )	// ビルボードの移動量
#define BILLBOARD_SIZE              ( 10 )		// テクスチャのサイズ
#define BILLBOARD_NUM               ( 15 )		// ビルボードの数

//============================================================================
//
// 構造体定義
//
//============================================================================

/* ビルボード用 構造体 */
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBtxBuffer;	// バーテックスバッファへのポインタ
	LPDIRECT3DTEXTURE9      pD3DTexture;		// テクスチャ

	D3DXMATRIX mtxWorld;	// ワールドマトリックス

	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	D3DXVECTOR3 scl;		// 大きさ

	bool bUse;

	int nShadowNumber;
	float fShadowSize;

	D3DXVECTOR3 Move;
}BILLBOARD;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitBillboard(void);	// ビルボードの初期化
void DrawBillboard(void);		// ビルボードの描画処理
void UpdataBillboard(void);		// ビルボードの更新処理
void UnInitBillboard(void);		// ビルボードの開放処理

BILLBOARD* GetBillboard(void);			// ビルボードの位置取得
void SetBullet( D3DXVECTOR3 pos, D3DXVECTOR3 rot );

/* インクルードガードの最終行 */
#endif	// _BILLBOARD_H_