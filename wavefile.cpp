//=============================================================================================================
//
// WAVEファイル関連コードファイル
// 制作者：根子 晴行
//
//=============================================================================================================
#include "wavefile.h"

#if	1	//本来は下記マクロ定義はメモリ関連のコードファイル等にあるべき！//
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//-------------------------------------
//	メモリ確保マクロ（型キャスト版）
//引数：	p		…	[out]	確保したメモリのアドレスを受け取るポインタ変数
//			type	…	[in]	データ型
//			n		…	[in]	確保するデータの個数
//			ptype	…	[in]	確保したメモリのアドレスをキャストする為のデータ型
//戻値：	無
//-------------------------------------
#define MEM_NEW_ARY_TYPECAST( p, type, n, ptype )	{ ( p ) = (ptype)(new type[ n ]); }
//-------------------------------------
//	メモリ解放マクロ
//引数：	p		…	[in]	確保したメモリのアドレスを保持しているポインタ変数
//戻値：	無
//-------------------------------------
#define MEM_DEL_ARY(          p                 )	{ delete[] ( p ); }
//-------------------------------------
//	メモリ解放マクロ（NULLチェック版）
//引数：	p		…	[i/o]	確保したメモリのアドレスを保持しているポインタ変数
//戻値：	無
//-------------------------------------
#define MEM_DEL_ARY_SAFE(     p                 )	{ if( NULL != p ) { MEM_DEL_ARY( p ); ( p ) = NULL; } }
#endif

//-------------------------------------
// コンストラクタ（デフォルト）
//-------------------------------------
CWaveFile::CWaveFile( void )
{
	this->m_data_offset	= 0;
	this->m_pwfx		= NULL;
	this->m_hmmio		= NULL;
	this->m_dwSize		= 0;
}

//-------------------------------------
// デストラクタ
//-------------------------------------
CWaveFile::~CWaveFile( void )
{
	this->Close();
}

//-------------------------------------
// WAVEファイルオープン
//引数：	char *filename	…	[in]	WAVEファイル名
//戻値：	BOOL			…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	CWaveFile::Open( char *filename )
{
	HRESULT hr;
	// WAVEファイルのオープン
	this->m_hmmio = mmioOpen( filename, NULL, MMIO_ALLOCBUF | MMIO_READ );
	if( NULL == this->m_hmmio )
	{
		return( FALSE );
	}
	// WAVEファイルの読み込み
	hr = this->ReadMMIO();
	if( FAILED( hr ) )
	{	// WAVEファイルの読み込みに失敗
		mmioClose( this->m_hmmio, 0 );
		return( FALSE );
	}
	// WAVEファイルのリセット
	hr = this->ResetFile();
	if( FAILED( hr ) )
	{
		mmioClose( this->m_hmmio, 0 );
		return( FALSE );
	}
	//データサイズを取得
	this->m_dwSize    = this->m_ck.cksize;
	//データチャンクまでのオフセットを取得
	this->m_data_offset = mmioSeek( this->m_hmmio, 0, SEEK_CUR );
	return( TRUE );
}

//-------------------------------------
// WAVEファイルを閉じる
//引数：	無
//戻値：	BOOL			…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	CWaveFile::Close( void )
{
	if( NULL != this->m_hmmio )
	{
		mmioClose( this->m_hmmio, 0 );
		this->m_hmmio = NULL;
	}
	MEM_DEL_ARY_SAFE( this->m_pwfx );
	return( TRUE );
}

//-------------------------------------
// ファイルをリセット
//引数：	無
//戻値：	HRESULT			…			成功時S_OK、エラー時それ以外
//-------------------------------------
HRESULT CWaveFile::ResetFile( void )
{
	if( NULL == this->m_hmmio )
	{
		return( CO_E_NOTINITIALIZED );
	}
	// ファイル位置を変更
	if( -1 == mmioSeek( this->m_hmmio, this->m_ckRiff.dwDataOffset + sizeof( FOURCC ), SEEK_SET ) )
	{
		return( E_FAIL );
	}
	// RIFFチャンクサーチ
	this->m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if( 0 != mmioDescend( this->m_hmmio, &this->m_ck, &this->m_ckRiff, MMIO_FINDCHUNK ) )
	{
		return( E_FAIL );
	}
	return( S_OK );
}

//-------------------------------------
// WAVEファイルを読み込む
//引数：	無
//戻値：	HRESULT			…			成功時S_OK、エラー時それ以外
//-------------------------------------
HRESULT CWaveFile::ReadMMIO( void )
{
	MMCKINFO		ckIn;
	PCMWAVEFORMAT	pcmWaveFormat;
	this->m_pwfx = NULL;
	if( ( 0 != mmioDescend( this->m_hmmio, &this->m_ckRiff, NULL, 0 ) ) )
	{
		return( E_FAIL );
	}
	// 読み込んだファイルがWAVEファイルかチェック
	if( ( this->m_ckRiff.ckid    != FOURCC_RIFF )
	||	( this->m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
	{
		return( E_FAIL );
	}
	// RIFFチャンクサーチ
	ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( 0 != mmioDescend( this->m_hmmio, &ckIn, &this->m_ckRiff, MMIO_FINDCHUNK ) )
	{
		return( E_FAIL );
	}
	// 正しくファイルが読み込まれているか、一部のメンバの値を参照してチェック
	if( ckIn.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )
	{
		return( E_FAIL );
	}
	// WAVEファイルのフォーマット情報をpcmWaveFormatに読み込む
	if( mmioRead( this->m_hmmio, ( HPSTR )&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )
	{
		return( E_FAIL );
	}
	// データを読み込む
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{	//PCMフォーマット
		MEM_NEW_ARY_TYPECAST( this->m_pwfx, CHAR, sizeof( WAVEFORMATEX ), WAVEFORMATEX * );
		if( NULL == this->m_pwfx )
		{
			return( E_FAIL );
		}
		memcpy( this->m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = 0;
	}
	else
	{	//拡張フォーマット
		WORD	cbExtraBytes = 0L;
		if( mmioRead( this->m_hmmio, ( CHAR* )&cbExtraBytes, sizeof( WORD ) ) != sizeof( WORD ) )
		{
			return( E_FAIL );
		}
		MEM_NEW_ARY_TYPECAST( this->m_pwfx, CHAR, sizeof( WAVEFORMATEX ) + cbExtraBytes, WAVEFORMATEX * );
		if( NULL == this->m_pwfx )
		{
			return( E_FAIL );
		}
		memcpy( this->m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		this->m_pwfx->cbSize = cbExtraBytes;
		if( mmioRead( this->m_hmmio, ( CHAR* )( ( ( BYTE* )&( this->m_pwfx->cbSize ) ) + sizeof( WORD ) ), cbExtraBytes ) != cbExtraBytes )
		{
			return( E_FAIL );
		}
	}
	// RIFFチャンクから退出
	if( 0 != mmioAscend( this->m_hmmio, &ckIn, 0 ) )
	{
		return( E_FAIL );
	}
	return( S_OK );
}

//-------------------------------------
// WAVEファイルを読み込む
//引数：	BYTE	*	pBuffer			…	[out]	読み込み先のバッファポインタ
//			DWORD		dwSizeToRead	…	[in]	読み込みたいサイズ
//			DWORD	*	pdwSizeRead		…	[out]	実際に読み込んだサイズ
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	CWaveFile::Read( BYTE* pBuffer, DWORD dwSizeToRead, DWORD* pdwSizeRead )
{
	MMIOINFO	mmioinfoIn;
	if( NULL == this->m_hmmio )
	{
		return( FALSE );
	}
	if( NULL == pBuffer
	||	NULL == pdwSizeRead )
	{
		return( FALSE );
	}
	*pdwSizeRead = 0;
	if( 0 != mmioGetInfo( this->m_hmmio, &mmioinfoIn, 0 ) )
	{
		return( FALSE );
	}
	UINT		cbDataIn = dwSizeToRead;
	if( cbDataIn > this->m_ck.cksize )
	{
		cbDataIn = this->m_ck.cksize;
	}
	this->m_ck.cksize -= cbDataIn;
	DWORD		cT;
	for( cT = 0; cT < cbDataIn; ++ cT )
	{
		if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
		{
			if( 0 != mmioAdvance( this->m_hmmio, &mmioinfoIn, MMIO_READ ) )
			{
				return( FALSE );
			}
			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead )
			{
				return( FALSE );
			}
		}
		*( ( BYTE * )pBuffer + cT ) = *( ( BYTE * )mmioinfoIn.pchNext );
		++ mmioinfoIn.pchNext;
	}
	if( 0 != mmioSetInfo( this->m_hmmio, &mmioinfoIn, 0 ) )
	{
		return( FALSE );
	}
	*pdwSizeRead = cbDataIn;
	return( TRUE );
}

//-------------------------------------
// シーク関数
//引数：	int			offset			…	[in]	オフセット
//			int			origin			…	[in]	オフセットの解釈方法フラグ(SEEK_~)
//戻値：	BOOL						…			成功時TRUE、エラー時FALSE
//-------------------------------------
BOOL	CWaveFile::Seek( int offset, int origin )
{
	if( -1 == mmioSeek( this->m_hmmio, offset, origin ) )
	{
		return( FALSE );
	}
	this->m_ck.cksize -= offset;
	return( TRUE );
}
