//=============================================================================================================
//
// WAVE�t�@�C���֘A�R�[�h�t�@�C��
// ����ҁF���q ���s
//
//=============================================================================================================
#include "wavefile.h"

#if	1	//�{���͉��L�}�N����`�̓������֘A�̃R�[�h�t�@�C�����ɂ���ׂ��I//
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//-------------------------------------
//	�������m�ۃ}�N���i�^�L���X�g�Łj
//�����F	p		�c	[out]	�m�ۂ����������̃A�h���X���󂯎��|�C���^�ϐ�
//			type	�c	[in]	�f�[�^�^
//			n		�c	[in]	�m�ۂ���f�[�^�̌�
//			ptype	�c	[in]	�m�ۂ����������̃A�h���X���L���X�g����ׂ̃f�[�^�^
//�ߒl�F	��
//-------------------------------------
#define MEM_NEW_ARY_TYPECAST( p, type, n, ptype )	{ ( p ) = (ptype)(new type[ n ]); }
//-------------------------------------
//	����������}�N��
//�����F	p		�c	[in]	�m�ۂ����������̃A�h���X��ێ����Ă���|�C���^�ϐ�
//�ߒl�F	��
//-------------------------------------
#define MEM_DEL_ARY(          p                 )	{ delete[] ( p ); }
//-------------------------------------
//	����������}�N���iNULL�`�F�b�N�Łj
//�����F	p		�c	[i/o]	�m�ۂ����������̃A�h���X��ێ����Ă���|�C���^�ϐ�
//�ߒl�F	��
//-------------------------------------
#define MEM_DEL_ARY_SAFE(     p                 )	{ if( NULL != p ) { MEM_DEL_ARY( p ); ( p ) = NULL; } }
#endif

//-------------------------------------
// �R���X�g���N�^�i�f�t�H���g�j
//-------------------------------------
CWaveFile::CWaveFile( void )
{
	this->m_data_offset	= 0;
	this->m_pwfx		= NULL;
	this->m_hmmio		= NULL;
	this->m_dwSize		= 0;
}

//-------------------------------------
// �f�X�g���N�^
//-------------------------------------
CWaveFile::~CWaveFile( void )
{
	this->Close();
}

//-------------------------------------
// WAVE�t�@�C���I�[�v��
//�����F	char *filename	�c	[in]	WAVE�t�@�C����
//�ߒl�F	BOOL			�c			������TRUE�A�G���[��FALSE
//-------------------------------------
BOOL	CWaveFile::Open( char *filename )
{
	HRESULT hr;
	// WAVE�t�@�C���̃I�[�v��
	this->m_hmmio = mmioOpen( filename, NULL, MMIO_ALLOCBUF | MMIO_READ );
	if( NULL == this->m_hmmio )
	{
		return( FALSE );
	}
	// WAVE�t�@�C���̓ǂݍ���
	hr = this->ReadMMIO();
	if( FAILED( hr ) )
	{	// WAVE�t�@�C���̓ǂݍ��݂Ɏ��s
		mmioClose( this->m_hmmio, 0 );
		return( FALSE );
	}
	// WAVE�t�@�C���̃��Z�b�g
	hr = this->ResetFile();
	if( FAILED( hr ) )
	{
		mmioClose( this->m_hmmio, 0 );
		return( FALSE );
	}
	//�f�[�^�T�C�Y���擾
	this->m_dwSize    = this->m_ck.cksize;
	//�f�[�^�`�����N�܂ł̃I�t�Z�b�g���擾
	this->m_data_offset = mmioSeek( this->m_hmmio, 0, SEEK_CUR );
	return( TRUE );
}

//-------------------------------------
// WAVE�t�@�C�������
//�����F	��
//�ߒl�F	BOOL			�c			������TRUE�A�G���[��FALSE
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
// �t�@�C�������Z�b�g
//�����F	��
//�ߒl�F	HRESULT			�c			������S_OK�A�G���[������ȊO
//-------------------------------------
HRESULT CWaveFile::ResetFile( void )
{
	if( NULL == this->m_hmmio )
	{
		return( CO_E_NOTINITIALIZED );
	}
	// �t�@�C���ʒu��ύX
	if( -1 == mmioSeek( this->m_hmmio, this->m_ckRiff.dwDataOffset + sizeof( FOURCC ), SEEK_SET ) )
	{
		return( E_FAIL );
	}
	// RIFF�`�����N�T�[�`
	this->m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if( 0 != mmioDescend( this->m_hmmio, &this->m_ck, &this->m_ckRiff, MMIO_FINDCHUNK ) )
	{
		return( E_FAIL );
	}
	return( S_OK );
}

//-------------------------------------
// WAVE�t�@�C����ǂݍ���
//�����F	��
//�ߒl�F	HRESULT			�c			������S_OK�A�G���[������ȊO
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
	// �ǂݍ��񂾃t�@�C����WAVE�t�@�C�����`�F�b�N
	if( ( this->m_ckRiff.ckid    != FOURCC_RIFF )
	||	( this->m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) )
	{
		return( E_FAIL );
	}
	// RIFF�`�����N�T�[�`
	ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( 0 != mmioDescend( this->m_hmmio, &ckIn, &this->m_ckRiff, MMIO_FINDCHUNK ) )
	{
		return( E_FAIL );
	}
	// �������t�@�C�����ǂݍ��܂�Ă��邩�A�ꕔ�̃����o�̒l���Q�Ƃ��ă`�F�b�N
	if( ckIn.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) )
	{
		return( E_FAIL );
	}
	// WAVE�t�@�C���̃t�H�[�}�b�g����pcmWaveFormat�ɓǂݍ���
	if( mmioRead( this->m_hmmio, ( HPSTR )&pcmWaveFormat, sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) )
	{
		return( E_FAIL );
	}
	// �f�[�^��ǂݍ���
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM )
	{	//PCM�t�H�[�}�b�g
		MEM_NEW_ARY_TYPECAST( this->m_pwfx, CHAR, sizeof( WAVEFORMATEX ), WAVEFORMATEX * );
		if( NULL == this->m_pwfx )
		{
			return( E_FAIL );
		}
		memcpy( this->m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		m_pwfx->cbSize = 0;
	}
	else
	{	//�g���t�H�[�}�b�g
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
	// RIFF�`�����N����ޏo
	if( 0 != mmioAscend( this->m_hmmio, &ckIn, 0 ) )
	{
		return( E_FAIL );
	}
	return( S_OK );
}

//-------------------------------------
// WAVE�t�@�C����ǂݍ���
//�����F	BYTE	*	pBuffer			�c	[out]	�ǂݍ��ݐ�̃o�b�t�@�|�C���^
//			DWORD		dwSizeToRead	�c	[in]	�ǂݍ��݂����T�C�Y
//			DWORD	*	pdwSizeRead		�c	[out]	���ۂɓǂݍ��񂾃T�C�Y
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
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
// �V�[�N�֐�
//�����F	int			offset			�c	[in]	�I�t�Z�b�g
//			int			origin			�c	[in]	�I�t�Z�b�g�̉��ߕ��@�t���O(SEEK_~)
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
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
