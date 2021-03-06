//=============================================================================================================
//
// COM(Component Object Model)関連コードファイル
//
//=============================================================================================================
#ifndef _COM_H_	// インクルードガード開始
#define _COM_H_	// インクルードガード用マクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <windows.h>	//共通ヘッダー

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//-------------------------------------
//COM初期化
//引数：	無
//戻値：	BOOL			…	成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	InitCOM( void );
//-------------------------------------
//COM初期化
//引数：	無
//戻値：	BOOL			…	成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	UnInitCOM( void );

#endif//_FONT_H_	// インクルードガード終了
