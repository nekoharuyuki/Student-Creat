//===============================================================================================================
//
// readygo.h
// author : Haruyuki Neko
//
//===============================================================================================================

// 画像全般ヘッダー

/* インクルードガードの先頭 */
#ifndef _READY_GO_H_
#define _READY_GO_H_

//============================================================================
//
// マクロ定義
//
//============================================================================

// テクスチャファイル名
#define TEXTURE_READY_FILE        "data/TEXTURE/ready.png"
#define TEXTURE_GO_FILE           "data/TEXTURE/go.png"

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitReadGo(void);	// 初期化 関数
void UpdateReadGo(void);	// 更新処理 関数
void DrawReadGo(void);		// 描画処理 関数
void UninitReadGo(void);	// 開放処理 関数

/* インクルードガードの最終行 */
#endif	// _READY_GO_H_