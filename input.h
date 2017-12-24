//===============================================================================================================
//
// input.h
// author : Haruyuki Neko
//
//===============================================================================================================

/* インクルードガードの先頭 */
#ifndef _INPUT_H_
#define _INPUT_H_

//============================================================================
//
// インクルード
//
//============================================================================

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION ( 0x0800 )	// 警告を一時的に無効にする
#endif

#include <dinput.h>

//============================================================================
//
// 関数プロトタイプ宣言
//
//============================================================================

HRESULT InitDInput(HINSTANCE hInstance, HWND hWnd);		// DirectInput 初期化関数
void ReleaseDInput(void);								// DirectInputデバイスとオブジェクトの開放 関数
void UpdataDInput(void);								// DirectInput 更新関数
bool nowKey(BYTE cKey);									// キー入力判定
bool PushKey(BYTE cKey);								// キー入力判定（ 押した瞬間 ）
bool ReleaseKey(BYTE cKey);								// キー入力判定（ 離した瞬間 ）

/* インクルードガードの最終行 */
#endif	// _INPUT_H_