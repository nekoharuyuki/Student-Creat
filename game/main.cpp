//===============================================================================================================
//
// main.cpp
// author : Haruyuki Neko
//
//===============================================================================================================

//============================================================================
//
// インクルード
//
//============================================================================

#include "main.h"

// システム
#include "directX3d.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"

// インプットに関するヘッダーファイル
#include "input.h"
#include "mouse.h"

// サウンドに関するヘッダーファイル
#include "com.h"
#include "sound.h"
#include "snddata.h"
#include "wavefile.h"

// ゲーム
#include "model.h"
#include "shadow.h"
#include "billboard.h"
#include "explosion.h"
#include "number.h"
#include "goal.h"

// シーン
#include "game.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "gameover.h"
#include "gameclear.h"
#include "readygo.h"

// フィールドの形成
#include "meshfield.h"
#include "meshwall.h"
#include "meshceiling.h"

//============================================================================
//
// グローバル変数
//
//============================================================================

static int g_nCountFPS;		// FPSのカウント
static int g_nTime;			// 時間

//============================================================================
//
// ウインドウプロシージャ関数（コールバック関数）
// 引数：
//　hWnd    …  ウインドウのハンドル
//　uMsg    …  メッセージの識別子（WM_~）
//　wParam  …  メッセージの最初のパラメータ
//　lParam  …  メッセージの番目のパラメータ
// 戻り値：
//　メッセージの処理結果
//
//============================================================================

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
//	case WM_SETCURSOR:
//			SetCursor( NULL );								// Windowsの標準カーソルを消す
//			return 1;
	case WM_CREATE:	// CreateWindow()呼出時
		break;
	case WM_KEYDOWN:
		switch( wParam )
		{
		case VK_ESCAPE:										// [ESC]キーが押された
/*
			if( MessageBox( NULL, "本当に終了しますか？", "確認", MB_YESNO ) == IDNO )	// メッセージボックス
			{	// キャンセルを選択
				return 0; // 閉じないためにはDefWindowProc()に処理させてはならない
			}
*/
			DestroyWindow( hWnd );							// ウインドウを破棄するよう指示する
			break;
		}
		break;
	case WM_CLOSE:    // ウィンドウ（又はアプリケーション）が終了しなければならない時
/*
		if( MessageBox( NULL, "本当に終了しますか？", "確認", MB_YESNO ) == IDNO )	// メッセージボックス
		{	// キャンセルを選択
			return 0; // 閉じないためにはDefWindowProc()に処理させてはならない
		}
*/
		break;  // 閉じて良いのならDefWindowProc()にまかせれば良い
	case WM_DESTROY:	// （閉じるボタンを押す等して）ウインドウ破棄時
		PostQuitMessage( 0 );	// "WM_OUIT"メッセージを返す
		break;
	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );	// 既定の処理を提供
}

//============================================================================
//
// メイン関数
// 引数：
//　hInstance     … このプログラムを識別するためのハンドル
//　hPrevInstance … 常にNULL
//　lpCmdLine     … コマンドラインから受け取った引数
//　nCmdShow      … アプリケーションの初期表示方法を指定
// 戻り値：
//　終了コード
//
//============================================================================

int APIENTRY WinMain( HINSTANCE	hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HRESULT hr;

	// フレームカウント用変数
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	timeBeginPeriod( 1 );	// ハードウェアタイマー精度を初期化（Windows用の関数）

	dwExecLastTime = dwFPSLastTime = timeGetTime();	// 現在時刻の取得（Windows用の関数）
	dwCurrentTime = dwFrameCount = 0;

	WNDCLASSEX wcex =
	{
		sizeof( WNDCLASSEX ),       	// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,                 	// 表示するウインドウのスタイルを指定
		WndProc,			            // ウインドウプロシージャのアドレスを指定
		0,                          	// 通常は使用しないので"0"を指定
		0,                          	// 通常は使用しないので"0"を指定
		hInstance,                  	// WinMainのパラメータのインスタンスハンドルを設定
		NULL,                       	// 使用するアイコンを指定（Windowsがもっているアイコンを使うならNULL）
		LoadCursor( NULL, IDC_ARROW ),	// マウスカーソルを指定
		( HBRUSH )( COLOR_WINDOW + 1 ),	// ウインドウのクライアント領域の背景色を指定
		NULL,                       	// Windowにつけるメニューを設定
		CLASS_NAME,                 	// ウインドウクラスの名前
		NULL                        	// 小さいアイコンが設定された場合の情報を記述
	};
	
	// ウインドウクラスの登録
	RegisterClassEx( &wcex );
	
	// ウインドウを作成
	HWND hWnd;
	hWnd = CreateWindow( CLASS_NAME,      		// ウインドウクラスの名前
						 WINDOW_NAME,		 	// ウインドウの名前
						 WS_OVERLAPPEDWINDOW,	// ウインドウスタイル
						 CW_USEDEFAULT,			// ウインドウの左上Ｘ座標
						 CW_USEDEFAULT,   		// ウインドウの左上Ｙ座標
						 SCREEN_WIDTH,    		// ウインドウの幅
						 SCREEN_HEIGHT,			// ウインドウの高さ
						 NULL,					// 親ウインドウのハンドル
						 NULL,					// メニューハンドルまたは子ウインドウID
						 hInstance,				// インスタンスハンドル
						 NULL );			  	// ウインドウ作成データ

	// DirectXの初期化
	hr = InitDX( hWnd );

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	// DirectXデバイスの値を取得
	LPDIRECT3DDEVICE9 D3DDevice = GetD3DDevice();

	// レンダリングステートパラメータの設定
	D3DDevice -> SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );				// カリングを行う
	D3DDevice -> SetRenderState( D3DRS_ZENABLE, TRUE );						// Zバッファを使用
	D3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );			// αブレンドを使用
	D3DDevice -> SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// αソースカラーの指定
	D3DDevice -> SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// αディスティネーションカラーの指定
	
	// サンプラーステートパラメータの設定
	D3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );	// テクスチャU値の繰返設定
	D3DDevice -> SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );	// テクスチャV値の繰返設定
	D3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// テクスチャ拡大時の補間設定
	D3DDevice -> SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );	// テクスチャ拡小時の補間設定

	// ウインドウの表示
	ShowWindow( hWnd, nCmdShow );	// 指定されたウインドウの表示状態を設定（ウインドウを表示）
	UpdateWindow( hWnd );       	// ウインドウの状態を直ちに反映（ウインドウのクライアント領域を描写）

	//--------------------
	// 各種初期化処理
	//--------------------

	// 各種初期化
	hr = Init();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

	// DirectInputに関する初期化
	hr = InitDInput( hInstance, hWnd );	// キーボード

	if( FAILED( hr ) )
	{
		// キーボードが接続されていません。
	}

	hr = InitDMouse( hInstance, hWnd );	// マウス

	if( FAILED( hr ) )
	{
		// マウスが接続されていません。
	}

	// COM初期化
	InitCOM();

	// サウンド初期化処理
	InitSnd( GetSoundFileParam( SOUND_LABEL_ST ), SOUND_LABEL_NUM );

	//--------------------

	// メッセージループ（メインループ）
	MSG msg;

	int a = timeGetTime();

	while( 1 )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( WM_QUIT == msg.message )
			{	// PeekMessage()が呼ばれたら
				break;	// ループ終了
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage( &msg );	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage( &msg );	// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{
			// フレームカウント
			dwCurrentTime = timeGetTime();
		
			if( ( dwCurrentTime - dwFPSLastTime ) >= 500 )
			{
				g_nCountFPS = dwFrameCount * 1000 / ( dwCurrentTime - dwFPSLastTime );
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if( ( dwCurrentTime - dwExecLastTime ) >= ( 1000 / 60 ) )
			{
				dwExecLastTime = dwCurrentTime;

				// バックバッファ＆Zバッファのクリア
				D3DDevice -> Clear( 0,
									NULL,
									( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ),
									D3DCOLOR_RGBA( 0, 0, 0, 0 ),			// 背景の色 設定
									1.0f,
									0 );
				
				hr = D3DDevice -> BeginScene();

				if( SUCCEEDED( hr ) )
				{
					// 開始からの時間取得
					int b = timeGetTime();
					g_nTime = ( b - a ) / 1000;

					//--------------------
					// 各種更新処理
					//--------------------

					// DirectInputの更新処理
					UpdataDInput();	// キーボード
					UpdataDMouse(hWnd);	// マウス

					Updata();

					//--------------------
					
					//--------------------
					// ゲームのメイン部分
					//--------------------

					GameMain();

					//--------------------

					//--------------------
					// 各種描画処理
					//--------------------

					Draw();

					//--------------------

#ifdef _DEBUG	//デバッグ用コード開始

					PrintDebugProc("FPS : %d\n", g_nCountFPS);

#endif //_DEBUG	//デバッグ用コード終了

					// Direct3Dによる描画の終了
					D3DDevice -> EndScene();

					// バックバッファとフロントバッファの入れ替え
					D3DDevice -> Present( NULL, NULL, NULL, NULL );
				}
				dwFrameCount++;
			}
		}
	}

	timeEndPeriod( 1 );	// ハードウェアタイマを元の状態へ戻しておく

	//--------------------
	// 各種終了処理
	//--------------------

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ用表示の終了処理
	UnInitDebugProc();

#endif //_DEBUG	//デバッグ用コード終了

	// カメラの終了処理
	UnInitCamera();

	// DirectInputの終了処理
	ReleaseDInput();	// キーボード
	ReleaseDMouse();	// マウス

	// フェードの終了処理
	UninitFade();

	// ゲームで使用したものの終了処理
	UnInit();

	// DirectXオブジェクトの開放
	ReleaseD3D();

	// COM終了処理
	UnInitCOM();

	// サウンド終了処理
	UninitSnd();

	//--------------------

	// ウインドウクラスの登録を解除
	UnregisterClass( CLASS_NAME, wcex.hInstance );

	return( int )msg.wParam;
}

//============================================================================
//
// 初期化 関数
// 引数：
//　なし
// 戻り値：
//	 S_OK	: 初期化に成功
//	 E_FAIL	: 初期化に失敗
//
//============================================================================

HRESULT Init(void)
{
	HRESULT hr;

	// カメラの初期化
	InitCamera();

	// ライトの初期化
	InitLight();

	// フェードの初期化
	hr = InitFade();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ表示の初期化
	hr = InitDebugProc();

	if( FAILED( hr ) )
	{
		return E_FAIL;	// 失敗した場合
	}

#endif //_DEBUG	//デバッグ用コード終了

	return S_OK;
}

//============================================================================
//
// 更新処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void Updata(void)
{
	// カメラの更新
	UpdataCamera();

	// フェードの更新
	UpdateFade();
}

//============================================================================
//
// 描画処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void Draw(void)
{
	// カメラの描画
	DrawCamera();

	// フェードの描画
	DrawFade();

#ifdef _DEBUG	//デバッグ用コード開始

	// デバッグ表示
	DrawDebugProc();

#endif //_DEBUG	//デバッグ用コード終了

}

//============================================================================
//
// 開放処理 関数
// 引数：
//　なし
// 戻り値：
//　なし
//
//============================================================================

void UnInit(void)
{
	/* ゲームの終了処理 */

	// ゲーム遷移系の終了処理
	UninitTitle();
	UninitTutorial();
	UninitGameClear();
	UninitGameOver();

	// メインゲームの終了処理

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

	// スコアの終了処理
	UninitNumber();

	// ゴールの終了処理
	UnInitGoal();

	UninitReadGo();
}

//============================================================================
//
// 時間取得 処理 関数
// 引数：
//　なし
// 戻り値：
//　g_nTime
//
//============================================================================

int GetTime(void)
{
	return g_nTime;
}

/* EOF */