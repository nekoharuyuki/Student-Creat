//===============================================================================================================
//
// number.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _NUMBER_H_
#define _NUMBER_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// 読み込むテクスチャファイル名
#define	TEXTURE_NUMBER		"data/texture/num_0001.png"

// 数字最大数
#define	MAX_NUM_NUMBER		( 20 )

// スコアの表示ポジション
#define	NUMBER_POS_X		( SCREEN_WIDTH - 100 )	// スコアの表示ポジション( X )
#define	NUMBER_POS_Y		( 50 )					// スコアの表示ポジション( Y )

// フレームの表示ポジション
#define	FRAME_POS_X		( SCREEN_WIDTH - 10 )	// フレームの表示ポジション( X )
#define	FRAME_POS_Y		( 30 )					// フレームの表示ポジション( Y )

//============================================================================
//
// 構造体定義
//
//============================================================================

/* 数値の構造体 */
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	int nValue;             // 数値
    int nType;              // 種類
	bool bUse;				// 使用可能かどうか
} NUMBER;

/* フレームの構造体 */
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	bool bUse;				// 使用可能かどうか
}FRAME;

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitNumber(void);	// 初期化 関数
void UpdateNumber(void);	// 更新処理 関数
void DrawNumber(void);		// 描画処理 関数
void UninitNumber(void);	// 開放処理 関数

void SetScore(int n);		// スコアの設定 関数

/* インクルードガードの最終行 */
#endif	// _NUMBER_H_
