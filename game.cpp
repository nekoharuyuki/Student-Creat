//===============================================================================================================
//
// game.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// インクルード
//
//============================================================================

#include "main.h"
#include "directX3d.h"
#include "debugproc.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

// サウンドに関するヘッダーファイル
#include "com.h"
#include "sound.h"
#include "snddata.h"

// ゲーム
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"
#include "goal.h"
#include "readygo.h"

// シーン
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "gameover.h"
#include "gameclear.h"
#include "ranking.h"

// フィールドの形成
#include "dungeon.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

// ゲーム遷移
static GAMEMODE g_GameMode = SCENE_TITLE;

// ゲームの時間取得
static int g_nCountTime = 0;
static int a = 0;
static int b = 0;

// 初期化フラグ
static bool g_bInitTitleFlg      = false;		// タイトル
static bool g_bInitTutorialFlg   = false;		// チュートリアル
static bool g_bInitGameFlg       = false;		// ゲーム
static bool g_bInitGameOverFlg   = false;		// ゲームオーバー
static bool g_bInitGameClearFlg  = false;		// ゲームクリア
static bool g_bInitGameRecordFlg = false;		// ゲームレコード

#ifdef _DEBUG	//デバッグ用コード開始

static bool bOn = false;
static int nTimeTemp = 0;

#endif //_DEBUG	//デバッグ用コード終了

//=============================================================================
//
// ゲームのメイン部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void GameMain(void)
{
	// 画面遷移
	switch( g_GameMode )
	{
	// タイトル
	case SCENE_TITLE:

		Title();

		break;

	// チュートリアル
	case SCENE_TUTORIAL:

		Tutorial();

		break;

	// ゲーム
	case SCENE_GAME:

		Game();

		break;
	
	// ゲームオーバー
	case SCENE_GAMEOVER:

		GameOver();

		break;

	// ゲームクリア
	case SCENE_CLEAR:

		GameClear();

		break;

	// ゲームの記録
	case SCENE_RECORD:

		GameRecord();

		break;

	case SCENE_MAX:

		UnInit();

		g_bInitTitleFlg      = false;		// タイトル
		g_bInitTutorialFlg   = false;		// チュートリアル
		g_bInitGameFlg       = false;		// ゲーム
		g_bInitGameOverFlg   = false;		// ゲームオーバー
		g_bInitGameClearFlg  = false;		// ゲームクリア
		g_bInitGameRecordFlg = false;		// ゲームレコード

#ifdef _DEBUG	//デバッグ用コード開始

		bOn = false;
		nTimeTemp = 0;

#endif //_DEBUG	//デバッグ用コード終了

		SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );
		//SetMode( SCENE_TITLE );

		break;
	}
}

//=============================================================================
//
// ゲームのタイトル部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void Title(void)
{
	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitTitleFlg == false )
	{
		// サウンド再生
		PlaySnd( SOUND_LABEL_BGM000 );

		InitTitle();

		g_bInitTitleFlg = true;
	}

	//--------------------

	//--------------------
	// 更新処理
	//--------------------

	UpdateTitle();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	DrawTitle();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		// サウンド再生
		PlaySnd( SOUND_LABEL_SE003 );

		SetFade( FADE_OUT, FADE_IN, SCENE_TUTORIAL );

		FADE Fade = GetFade();

		if( Fade == FADE_OUT )
		{
			// タイトルの終了処理
			UninitTitle();
		}

	}

	//--------------------
}

//=============================================================================
//
// チュートリアル部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void Tutorial(void)
{
	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitTutorialFlg == false )
	{
		InitTutorial();

		g_bInitTutorialFlg = true;
	}

	//--------------------

	//--------------------
	// 更新処理
	//--------------------

	UpdateTutorial();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	DrawTutorial();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		// サウンド再生
		PlaySnd( SOUND_LABEL_SE003 );

		StopSnd( SOUND_LABEL_BGM000 );

		SetFade( FADE_OUT, FADE_IN, SCENE_GAME );

		FADE Fade = GetFade();

		if( Fade == FADE_OUT )
		{
			// チュートリアルの終了処理
			UninitTutorial();
		}

//		SetMode( SCENE_GAME );
	}

	//--------------------
}

//=============================================================================
//
// ゲーム部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void Game(void)
{
	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitGameFlg == false )
	{
		InitReadGo();

		SelectDungeon();

		// 地面の初期化
		InitField();

		// 壁の初期化
		InitWall();

		// 天井の初期化
		InitCeiling();

		// 影の初期化
		InitShadow();

		// モデルの初期化
		InitModel();

		// ビルボードの初期化
		InitBillboard();

		// 爆発エフェクトの初期化
		InitExplosion();

		// スコアの初期化
		InitNumber();

		// ゴールの初期化
		InitGoal();

		a = 0;
		b = 0;
		g_nCountTime = 0;

		a = timeGetTime();

#ifdef _DEBUG	//デバッグ用コード開始

		nTimeTemp = 0;

#endif //_DEBUG	//デバッグ用コード終了

		// サウンド再生
		PlaySnd( SOUND_LABEL_BGM001 );
		PlaySnd( SOUND_LABEL_SE006 );

		g_bInitGameFlg = true;
	}

	//--------------------

	// モデルの情報取得
	MODEL *Model = GetModel();

#ifdef _DEBUG	//デバッグ用コード開始

	bool DebugFlg = DebugProcFlg();
	if( Model -> bGoal == false )
	{
		/* ゲーム開始からの経過時間を取得 */
		b = timeGetTime();

		// キー入力判定
		if( PushKey( DIK_F2 ) && DebugFlg == true )
		{
			if( bOn == false )
			{	// 時間を止める
				// 止めた時の時間保存
				nTimeTemp = timeGetTime();
				bOn = true;
			}
			else if( bOn == true )
			{	// 時間を動かす
				a = timeGetTime();
				bOn = false;
			}
		}

		// キー入力判定
		if( PushKey( DIK_G ) && DebugFlg == true )
		{
			SetMode( SCENE_CLEAR );
		}

		if( bOn )
		{
			g_nCountTime = ( nTimeTemp - a ) / 1000;
		}
		else
		{
			g_nCountTime = ( b - a ) / 1000;
		}

		g_nCountTime = TIME_LIMIT - g_nCountTime;

		// キー入力判定
		if( PushKey( DIK_BACKSPACE ) && DebugFlg == true )
		{
			g_nCountTime = 0;
		}
	}

	SetScore( g_nCountTime );

#else if			// リリースの時

	if( Model->bGoal == false )
	{
		/* ゲーム開始からの経過時間を取得 */
		b = timeGetTime();
		g_nCountTime = ( b - a ) / 1000;
		g_nCountTime = TIME_LIMIT - g_nCountTime;
	}

	SetScore( g_nCountTime );

#endif //_DEBUG	//デバッグ用コード終了

	//--------------------
	// 更新処理
	//--------------------

	// 地面の更新
	UpdataFiled();

	// 壁の更新
	UpdataWall();

	// 天井の更新
	UpdataCeiling();

	// 影の更新
	UpdataShadow();

	// ビルボードの更新
	UpdataBillboard();

	// 爆発エフェクトの更新
	UpdataExplosion();

	// モデルの更新
	UpdataModel();

	// スコアの更新
	UpdateNumber();

	// ゴールの更新
	UpdataGoal();

	// RedyGoの更新
	UpdateReadGo();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	// 地面の描画
	DrawField();

	// 壁の描画
	DrawWall();

	// 天井の描画
	DrawCeiling();

	// 影の描画
	DrawShadow();

	// モデルの描画
	DrawModel();

	// ビルボードの描画
	DrawBillboard();

	// 爆発エフェクトの描画
	DrawExplosion();

	if( Model->bGoal == false )
	{	// ゴールへ到着していない場合

		// ゴールの描画
		DrawGoal();

		// スコアの描画
		DrawNumber();
	}

	// ReadyGoの表示
	DrawReadGo();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	// ゲームクリア

	if( Model->bGoal == true )	// ゴールへ到着した場合
	{
		SetMode( SCENE_CLEAR );
	}

	// ゲームオーバー

	if( g_nCountTime == 0 )	// 制限時間になってしまった場合
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_GAMEOVER );

		// 地面の終了処理
		UnInitField();

		// 壁の終了処理
		UnInitWall();

		// 天井の終了処理
		UnInitCeiling();

		// ビルボードの終了処理
		UnInitBillboard();

		// モデルの終了処理
		UnInitModel();

		// 影の終了処理
		UnInitShadow();

		// 爆発エフェクトの終了処理
		UnInitExplosion();

		// ゴールの終了処理
		UnInitGoal();

		UninitReadGo();

		g_bInitGameFlg = false;

		SetMode( SCENE_GAMEOVER );
	}

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ情報表示
	if( bOn == true )
	{
		PrintDebugProc("TIME STOP \n");
	}
	else
	{
		PrintDebugProc("TIME MOVE \n");
	}

#endif //_DEBUG	//デバッグ用コード終了

	//--------------------
}

//=============================================================================
//
// ゲームの時間取得
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

int GetGameTime(void)
{
	return g_nCountTime;
}

//=============================================================================
//
// ゲームクリア部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void GameClear( void )
{
	// ゲーム
	Game();

	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitGameClearFlg == false )
	{
		// サウンド再生
		PlaySnd( SOUND_LABEL_SE004 );

		InitGameClear();

		g_bInitGameClearFlg = true;		// ゲームクリア
	}

	//--------------------

	//--------------------
	// 更新処理
	//--------------------

	UpdateGameClear();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	DrawGameClear();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

//		StopSnd( SOUND_LABEL_BGM001 );

		// 地面の終了処理
		UnInitField();

		// 壁の終了処理
		UnInitWall();

		// 天井の終了処理
		UnInitCeiling();

		// ビルボードの終了処理
		UnInitBillboard();

		// モデルの終了処理
		UnInitModel();

		// 影の終了処理
		UnInitShadow();

		// 爆発エフェクトの終了処理
		UnInitExplosion();

		// ゴールの終了処理
		UnInitGoal();

		// ゲームクリア表示の終了処理
		UninitGameClear();

		UninitReadGo();

		StopSnd( SOUND_LABEL_BGM001 );

		g_bInitGameClearFlg = false;
		g_bInitGameFlg      = false;

		SetMode( SCENE_RECORD );
	}

	//--------------------
}

//=============================================================================
//
// ゲームオーバー部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void GameOver(void)
{
	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitGameOverFlg == false )
	{
		StopSnd( SOUND_LABEL_BGM001 );

		// サウンド再生
		PlaySnd( SOUND_LABEL_BGM002 );

		InitGameOver();

		g_bInitGameOverFlg = true;
	}

	//--------------------

	//--------------------
	// 更新処理
	//--------------------

	UpdateGameOver();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	DrawGameOver();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

		StopSnd( SOUND_LABEL_BGM002 );

		UninitGameOver();

		// スコアの終了処理
		UninitNumber();

		g_bInitGameOverFlg = false;

		SetMode( SCENE_MAX );
	}

	//--------------------
}

//=============================================================================
//
// ゲームレコード部分
//
// 引数：
//　なし
// 戻り値：
//　なし
//
//=============================================================================

void GameRecord(void)
{
	//--------------------
	// 初期化処理
	//--------------------

	if( g_bInitGameRecordFlg == false )
	{
		InitGameRecord();

		g_bInitGameRecordFlg = true;
	}

	//--------------------

	//--------------------
	// 更新処理
	//--------------------

	UpdateGameRecord();

	SetScore( g_nCountTime );

	// スコアの更新
	UpdateNumber();

	//--------------------

	//--------------------
	// 描画処理
	//--------------------

	DrawGameRecord();

	// スコアの描画
	DrawNumber();

	//--------------------

	//--------------------
	// 終了処理
	//--------------------

	if( PushKey( DIK_RETURN ) || PushKey( DIK_SPACE ) || PushClick( LEFT ) )
	{
		//SetFade( FADE_OUT, FADE_IN, SCENE_TITLE );

		UninitGameRecord();

		// スコアの終了処理
		UninitNumber();

		g_bInitGameRecordFlg = false;

		SetMode( SCENE_MAX );
	}

	//--------------------
}

//============================================================================
//
// 次のゲーム遷移用変数セット 関数
// 引数：
//　なし
// 戻り値：
//　scene　…　ゲーム遷移 変数
//
//============================================================================

void SetMode( GAMEMODE next )
{
	g_GameMode = next;
}

GAMEMODE GetMode(void)
{
	return g_GameMode;
}


/* EOF */