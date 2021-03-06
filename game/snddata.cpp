//=============================================================================================================
//
// サウンドデータをまとめたコードファイル
// 制作者：根子 晴行
//
//=============================================================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "snddata.h"

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
//サウンドファイルパラメータテーブル
static SOUNDFILEPARAM SoundFileParam[ SOUND_LABEL_NUM ] =
{
	{ "data/BGM/sample000.wav", TRUE  },	// サンプルBGM(WAVファイル)
	{ "data/BGM/sample001.wav", TRUE  },	// サンプルBGM(WAVファイル)
	{ "data/BGM/GAMEOVER.wav" , TRUE  },	// サンプルBGM(WAVファイル)
	{ "data/SE/Attention.wav" , FALSE },	// サンプルSE
	{ "data/SE/Battle.wav"    , FALSE },	// サンプルSE
	{ "data/SE/Bomb.wav"      , FALSE },	// サンプルSE
	{ "data/SE/Click.wav"     , FALSE },	// サンプルSE
	{ "data/SE/Goal.wav"      , FALSE },	// サンプルSE
	{ "data/SE/Hit.wav"       , FALSE },	// サンプルSE
	{ "data/SE/ReadyGo.wav"   , FALSE },	// サンプルSE
	{ "data/SE/Whistle.wav"   , FALSE },	// サンプルSE
};

//*****************************************************************************
// 関数定義
//*****************************************************************************
//-------------------------------------
// サウンドファイルパラメータ取得関数
//引数：	SOUND_LABEL			id		…	[in]	指定サウンドファイルＩＤ
//戻値：	SOUNDFILEPARAM	*			…			指定サウンドファイルＩＤのパラメータポインタ
//-------------------------------------
SOUNDFILEPARAM *GetSoundFileParam( SOUND_LABEL id )
{
	return( &SoundFileParam[ id ] );
}
