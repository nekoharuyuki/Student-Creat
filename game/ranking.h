//===============================================================================================================
//
// ranking.h
// author : Haruyuki Neko
//
//===============================================================================================================

// 画像全般ヘッダー

/* インクルードガードの先頭 */
#ifndef _GAME_RANKING_H_
#define _GAME_RANKING_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// テクスチャファイル名
#define TEXTURE_RANKING_FILE        "data/TEXTURE/GameRanking.png"
#define TEXTURE_SECOND_FILE			"data/TEXTURE/GameSecond.png"

#define TEX_RANKING_SIZE_X          ( 10 )
#define TEX_RANKING_SIZE_Y          ( 10 )

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitGameRecord(void);		// 初期化 関数
void UpdateGameRecord(void);		// 更新処理 関数
void DrawGameRecord(void);		// 描画処理 関数
void UninitGameRecord(void);		// 開放処理 関数

/* インクルードガードの最終行 */
#endif	// _GAME_RANKING_H_