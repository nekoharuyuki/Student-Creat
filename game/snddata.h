//=============================================================================================================
//
// サウンドデータをまとめたコードファイル
// 制作者：根子 晴行
//
//=============================================================================================================
#ifndef _SNDDATA_H_	// インクルードガード開始
#define _SNDDATA_H_	// インクルードガード用マクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "sound.h"

//*****************************************************************************
// 列挙体定義
//*****************************************************************************
// サウンドファイルＩＤ
typedef enum
{
	SOUND_LABEL_ST  = 0,								// ＩＤの先頭
	SOUND_LABEL_BEF = SOUND_LABEL_ST  - 1,				// ＩＤの先頭前

	SOUND_LABEL_BGM000,									// サンプルBGM(WAVファイル)
	SOUND_LABEL_BGM001,									// サンプルBGM(WAVファイル)
	SOUND_LABEL_BGM002,									// サンプルBGM(WAVファイル)
	SOUND_LABEL_SE000,									// サンプルSE
	SOUND_LABEL_SE001,									// サンプルSE
	SOUND_LABEL_SE002,									// サンプルSE
	SOUND_LABEL_SE003,									// サンプルSE
	SOUND_LABEL_SE004,									// サンプルSE
	SOUND_LABEL_SE005,									// サンプルSE
	SOUND_LABEL_SE006,									// サンプルSE
	SOUND_LABEL_SE007,									// サンプルSE

	SOUND_LABEL_AFT,									// ＩＤのラスト後
	SOUND_LABEL_NUM = SOUND_LABEL_AFT - SOUND_LABEL_ST,	// ＩＤのラスト
	SOUND_LABEL_ED  = SOUND_LABEL_AFT - -1,				// ＩＤの数
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//-------------------------------------
// サウンドファイルパラメータ取得関数
//引数：	SOUND_LABEL			id		…	[in]	指定サウンドファイルＩＤ
//戻値：	SOUNDFILEPARAM	*			…			指定サウンドファイルＩＤのパラメータポインタ
//-------------------------------------
SOUNDFILEPARAM	*GetSoundFileParam( SOUND_LABEL id );

#endif//_SNDDATA_H_	// インクルードガード終了
