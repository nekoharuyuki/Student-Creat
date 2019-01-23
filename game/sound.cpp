//=============================================================================================================
//
// �T�E���h�֘A�̃R�[�h�t�@�C��
// ����ҁF���q ���s
//
//=============================================================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <xaudio2.h>
#include "wavefile.h"
#include "sound.h"

#if	1	//�{���͉��L�}�N����`�̓������֘A�̃R�[�h�t�@�C�����ɂ���ׂ��I//
//*****************************************************************************
// �}�N����`
//*****************************************************************************
//-------------------------------------
//	�������m�ۃ}�N��
//�����F	p		�c	[out]	�m�ۂ����������̃A�h���X���󂯎��|�C���^�ϐ�
//			type	�c	[in]	�f�[�^�^
//			n		�c	[in]	�m�ۂ���f�[�^�̌�
//�ߒl�F	��
//-------------------------------------
#define MEM_NEW_ARY(          p, type, n        )	{ ( p ) =         new type[ n ] ; }
//-------------------------------------
//	����������}�N��
//�����F	p		�c	[in]	�m�ۂ����������̃A�h���X��ێ����Ă���|�C���^�ϐ�
//�ߒl�F	��
//-------------------------------------
#define MEM_DEL_ARY(          p                 )	{ delete[] ( p ); }
#endif

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static IXAudio2					*	xaudio			=	NULL;	// XAudio�C���^�[�t�F�[�X
static IXAudio2MasteringVoice	*	mastering_voice	=	NULL;	// �}�X�^�����O�{�C�X

static SOUNDFILEPARAM			*	pParam			=	NULL;	// �T�E���h�t�@�C���p�����[�^�擪�|�C���^
static int							nParam			=	0;		// �T�E���h�t�@�C���p�����[�^��

static CWaveFile				*	pWaveFile		=	NULL;	// WAVE�t�@�C���Ǘ��N���X�擪�|�C���^

static BYTE						**	pAudioData		=	NULL;	// �I�[�f�B�I�f�[�^�擪�|�C���^

static IXAudio2SourceVoice		**	source_voice	=	NULL;	// �\�[�X�{�C�X�擪�|�C���^
//-------------------------------------
// �T�E���h����������
//�����F	SOUNDFILEPARAM	*	pParam	�c	[in]	�T�E���h�t�@�C���p�����[�^�̐擪�|�C���^
//			int					nParam	�c	[in]	�T�E���h�t�@�C���p�����[�^��
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
//-------------------------------------
BOOL	InitSnd( SOUNDFILEPARAM *pPar, int nPar )
{
	HRESULT hr;
	pParam = pPar;
	nParam = nPar;
	// XAudio�C���^�[�t�F�[�X�̍쐬
	UINT32	flags = 0;
#ifdef  _DEBUG	//�f�o�b�O�p�R�[�h�J�n
	flags |= XAUDIO2_DEBUG_ENGINE;	//�f�o�b�O���̓f�o�b�O�t���O�𗧂Ă�
#endif//_DEBUG	//�f�o�b�O�p�R�[�h�I��
	hr = XAudio2Create( &xaudio, flags );
	if( FAILED( hr ) )
	{	//XAudio�C���^�[�t�F�[�X�̍쐬�Ɏ��s
		return( FALSE );
	}
	// �}�X�^�����O�{�C�X�̍쐬
	hr = xaudio->CreateMasteringVoice( &mastering_voice );
	if( FAILED( hr ) )
	{	//�}�X�^�����O�{�C�X�̍쐬�Ɏ��s
		return( FALSE );
	}
	// ���[�N�������m��
	MEM_NEW_ARY( pWaveFile   , CWaveFile            , nParam );
	MEM_NEW_ARY( pAudioData  , BYTE                *, nParam );
	MEM_NEW_ARY( source_voice, IXAudio2SourceVoice *, nParam );
	// WAVE�t�@�C����ǂݍ���
	CWaveFile	*pWav;
	DWORD		size;
	int			cnt;
	BOOL		bi;
	for( cnt = 0; cnt < nParam; ++ cnt )
	{
		pWav	=	&pWaveFile[ cnt ];
		// WAVE�t�@�C���I�[�v��
		bi		=	pWav->Open( pPar[ cnt ].filename );
		if( FALSE == bi )
		{	//WAVE�t�@�C���I�[�v�����s
			return( FALSE );
		}
		// �\�[�X�{�C�X�̍쐬
		hr = xaudio->CreateSourceVoice( &source_voice[ cnt ], pWav->GetFormat() );
		if( FAILED( hr ) )
		{	//�\�[�X�{�C�X�쐬���s
			return( FALSE );
		}
		// WAV�t�@�C���̃T�C�Y���̃o�b�t�@���쐬���A�f�[�^����������
		size = pWav->GetSize();
		MEM_NEW_ARY( pAudioData[ cnt ], BYTE, size );
		bi = pWav->Read( pAudioData[ cnt ], size, &size );
		if( FALSE == bi )
		{	//�t�@�C���ǂݍ��ݎ��s
			return( FALSE );
		}
	}
	return( TRUE );
}
//-------------------------------------
// �T�E���h�I������
//�����F	��
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
//-------------------------------------
BOOL	UninitSnd( void )
{
	int cnt;
	//�\�[�X�{�C�X�j��
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
	//�o�b�t�@�����
	if( NULL != pAudioData )
	{
		for( cnt = 0; cnt < nParam; ++ cnt )
		{
			MEM_DEL_ARY( pAudioData[ cnt ] );
		}
		MEM_DEL_ARY( pAudioData );
		pAudioData = NULL;
	}
	//WAVE�t�@�C���Ǘ��N���X�j��
	if( NULL != pWaveFile )
	{
		for( cnt = 0; cnt < nParam; ++ cnt )
		{
			pWaveFile[ cnt ].Close();
		}
		MEM_DEL_ARY( pWaveFile );
		pWaveFile = NULL;
	}
	//�}�X�^�����O�{�C�X�j��
	if( NULL != mastering_voice )
	{
		mastering_voice->DestroyVoice();
		mastering_voice = NULL;
	}
	//XAudio�C���^�[�t�F�[�X�j��
	if( NULL != xaudio )
	{
		xaudio->Release();
		xaudio = NULL;
	}
	return( TRUE );
}
//-------------------------------------
// �T�E���h��~�֐�
//�����F	SOUND_LABEL			label	�c	[in]	�w��T�E���h�t�@�C���h�c
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
//-------------------------------------
BOOL	StopSnd( int label )
{
	if(( label <  0      )
	|| ( label >= nParam ) )
	{
		return( FALSE );
	}
	IXAudio2SourceVoice	*pSourceVoice = source_voice[ label ];
	// ���t��~
	pSourceVoice->Stop();
	// �o�b�t�@���N���A
	pSourceVoice->FlushSourceBuffers();
	return( TRUE );
}
//-------------------------------------
// �T�E���h�Đ��֐�
//�����F	SOUND_LABEL			label	�c	[in]	�w��T�E���h�t�@�C���h�c
//�ߒl�F	BOOL						�c			������TRUE�A�G���[��FALSE
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
	// �Đ����Ȃ�Β��f
	StopSnd( label );
	// SourceVoice�Ƀf�[�^�𑗐M
	XAUDIO2_BUFFER		submit = { 0 };
	submit.AudioBytes = pWaveFile[  label ].GetSize();	//�o�b�t�@�̃o�C�g��
	submit.pAudioData = pAudioData[ label ];			//�o�b�t�@�̐擪�A�h���X
	submit.Flags      = XAUDIO2_END_OF_STREAM;			// ���̃o�b�t�@�̌�ɃL���[���̃o�b�t�@�[�����݂ł��Ȃ����Ƃ�����
	pSourceVoice -> SubmitSourceBuffer( &submit );
	// ���t�Đ�
	hr = pSourceVoice ->Start();
	if( FAILED( hr ) )
	{
		return( FALSE );
	}
	return( TRUE );
}
