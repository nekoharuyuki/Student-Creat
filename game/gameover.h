//===============================================================================================================
//
// gameover.h
// author : Haruyuki Neko
//
//===============================================================================================================

// 画像全般ヘッダー

/* インクルードガードの先頭 */
#ifndef _GAME_OVER_H_
#define _GAME_OVER_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// テクスチャファイル名
#define TEXTURE_GAMEOVER_FILE        "data/TEXTURE/GameOver.png"
#define TEXTURE_CRYPIG_FILE	         "data/TEXTURE/pig_2.png"
#define TEXTURE_CRY_FILE	         "data/TEXTURE/GameOverTear.png"

#define TEX_PIG_SIZE                 ( 200 )

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitGameOver(void);		// 初期化 関数
void UpdateGameOver(void);		// 更新処理 関数
void DrawGameOver(void);		// 描画処理 関数
void UninitGameOver(void);		// 開放処理 関数

void Rotation(float Rota);

/* インクルードガードの最終行 */
#endif	// _GAME_OVER_H_