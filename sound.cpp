//=============================================================================================================
//
// サウンド関連のコードファイル
// 制作者：根子 晴行
//
//=============================================================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include <xaudio2.h>
#include "wavefile.h"
#include "sound.h"

#if	1	//本来は下記マクロ定義はメモリ関連のコードファイル等にあるべき！//
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//-------------------------------------
//	メモリ確保マクロ
//引数：	p		…	[out]	確保したメモリのアドレスを受け取るポインタ変数
//			type	…	[in]	データ型
//			n		…	[in]	確保するデータの個数
//戻値：	無
//-------------------------------------
#define MEM_NEW_ARY(          p, type, n        )	{ ( p ) =         new type[ n ] ; }
//-------------------------------------
//	メモリ解放マクロ
//引数：	p		…	[in]	確保したメモリのアドレスを保持しているポインタ変数
//戻値：	無
//-------------------------------------
#define MEM_DEL_ARY(          p                 )	{ delete[] ( p ); }
#endif

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static IXAudio2					*	xaudio			=	NULL;	// XAudioインターフェース
static IXAudio2MasteringVoice	*	mastering_voice	=	NULL;	// マスタリングボイス

static SOUNDFILEPARAM			*	pParam			=	NULL;	// サウンドファイルパラメータ先頭ポインタ
static int							nParam			=	0;		// サウンドファイルパラメータ数

static CWaveFile				*	pWaveFile		=	NULL;	// WAVEファイル管理クラス先頭ポインタ

static BYTE						**	pAudioData		=	NULL;	// オーディオデータ先頭ポインタ

static IXAudio2SourceVoice		**	source_voice	=	NULL;	// ソースボイス先頭ポインタ
//-------------------------------------
// サウンド初期化処理
//引数：	SOUNDFILEPARAM	*	pParam	…	[in]	サウンドファイルパラメータの先頭ポインタ
//			int					nParam	…	[in]	サウンドファイルパラメータ数
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	InitSnd( SOUNDFILEPARAM *pPar, int nPar )
{
	HRESULT hr;
	pParam = pPar;
	nParam = nPar;
	// XAudioインターフェースの作成
	UINT32	flags = 0;
#ifdef  _DEBUG	//デバッグ用コード開始
	flags |= XAUDIO2_DEBUG_ENGINE;	//デバッグ時はデバッグフラグを立てる
#endif//_DEBUG	//デバッグ用コード終了
	hr = XAudio2Create( &xaudio, flags );
	if( FAILED( hr ) )
	{	//XAudioインターフェースの作成に失敗
		return( FALSE );
	}
	// マスタリングボイスの作成
	hr = xaudio->CreateMasteringVoice( &mastering_voice );
	if( FAILED( hr ) )
	{	//マスタリングボイスの作成に失敗
		return( FALSE );
	}
	// ワークメモリ確保
	MEM_NEW_ARY( pWaveFile   , CWaveFile            , nParam );
	MEM_NEW_ARY( pAudioData  , BYTE                *, nParam );
	MEM_NEW_ARY( source_voice, IXAudio2SourceVoice *, nParam );
	// WAVEファイルを読み込む
	CWaveFile	*pWav;
	DWORD		size;
	int			cnt;
	BOOL		bi;
	for( cnt = 0; cnt < nParam; ++ cnt )
	{
		pWav	=	&pWaveFile[ cnt ];
		// WAVEファイルオープン
		bi		=	pWav->Open( pPar[ cnt ].filename );
		if( FALSE == bi )
		{	//WAVEファイルオープン失敗
			return( FALSE );
		}
		// ソースボイスの作成
		hr = xaudio->CreateSourceVoice( &source_voice[ cnt ], pWav->GetFormat() );
		if( FAILED( hr ) )
		{	//ソースボイス作成失敗
			return( FALSE );
		}
		// WAVファイルのサイズ分のバッファを作成し、データを書き込む
		size = pWav->GetSize();
		MEM_NEW_ARY( pAudioData[ cnt ], BYTE, size );
		bi = pWav->Read( pAudioData[ cnt ], size, &size );
		if( FALSE == bi )
		{	//ファイル読み込み失敗
			return( FALSE );
		}
	}
	return( TRUE );
}
//-------------------------------------
// サウンド終了処理
//引数：	無
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	UninitSnd( void )
{
	int cnt;
	//ソースボイス破棄
	if( NULL != source_voice )
	{
		for( cnt = 0; cnt < nParam; ++ cnt )
		{
			source_voice[ cnt ]->Stop();
			source_voice[ cnt ]->DestroyVoice();
		}
		MEM_DEL_ARY( source_voice );
		source_voice = NULL;
	}
	//バッファを解放
	if( NULL != pAudioData )
	{
		for( cnt = 0; cnt < nParam; ++ cnt )
		{
			MEM_DEL_ARY( pAudioData[ cnt ] );
		}
		MEM_DEL_ARY( pAudioData );
		pAudioData = NULL;
	}
	//WAVEファイル管理クラス破棄
	if( NULL != pWaveFile )
	{
		for( cnt = 0; cnt < nParam; ++ cnt )
		{
			pWaveFile[ cnt ].Close();
		}
		MEM_DEL_ARY( pWaveFile );
		pWaveFile = NULL;
	}
	//マスタリングボイス破棄
	if( NULL != mastering_voice )
	{
		mastering_voice->DestroyVoice();
		mastering_voice = NULL;
	}
	//XAudioインターフェース破棄
	if( NULL != xaudio )
	{
		xaudio->Release();
		xaudio = NULL;
	}
	return( TRUE );
}
//-------------------------------------
// サウンド停止関数
//引数：	SOUND_LABEL			label	…	[in]	指定サウンドファイルＩＤ
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	StopSnd( int label )
{
	if(( label <  0      )
	|| ( label >= nParam ) )
	{
		return( FALSE );
	}
	IXAudio2SourceVoice	*pSourceVoice = source_voice[ label ];
	// 演奏停止
	pSourceVoice->Stop();
	// バッファをクリア
	pSourceVoice->FlushSourceBuffers();
	return( TRUE );
}
//-------------------------------------
// サウンド再生関数
//引数：	SOUND_LABEL			label	…	[in]	指定サウンドファイルＩＤ
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	PlaySnd( int label )
{
	HRESULT	hr;
	if(( label <  0      )
	|| ( label >= nParam ) )
	{
		return( FALSE );
	}
	IXAudio2SourceVoice	*pSourceVoice = source_voice[ label ];
	// 再生中ならば中断
	StopSnd( label );
	// SourceVoiceにデータを送信
	XAUDIO2_BUFFER		submit = { 0 };
	submit.AudioBytes = pWaveFile[  label ].GetSize();	//バッファのバイト数
	submit.pAudioData = pAudioData[ label ];			//バッファの先頭アドレス
	submit.Flags      = XAUDIO2_END_OF_STREAM;			// このバッファの後にキュー内のバッファーが存在できないことを示す
	pSourceVoice -> SubmitSourceBuffer( &submit );
	// 演奏再生
	hr = pSourceVoice ->Start();
	if( FAILED( hr ) )
	{
		return( FALSE );
	}
	return( TRUE );
}
