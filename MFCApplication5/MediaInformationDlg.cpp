// MediaInformationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "MediaInformationDlg.h"
#include "afxdialogex.h"

extern "C"
{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libswscale/swscale.h"

};

// MediaInformationDlg �Ի���

IMPLEMENT_DYNAMIC(MediaInformationDlg, CDialog)

MediaInformationDlg::MediaInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MediaInformationDlg::IDD, pParent)
{

}

MediaInformationDlg::~MediaInformationDlg()
{
}

void MediaInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FORMAT_INPUTFORMAT, m_formatinputformat);
	DDX_Control(pDX, IDC_FORMAT_DURATION, m_formatduration);
	DDX_Control(pDX, IDC_FORMAT_BITRATE, m_formatbitrate);
	DDX_Control(pDX, IDC_CODECV_NAME, m_codecvname);
	DDX_Control(pDX, IDC_CODECV_FRAMERATE, m_codecvframerate);
	DDX_Control(pDX, IDC_CODECV_RESOLUTION, m_codecvresolution);
	DDX_Control(pDX, IDC_CODECA_CHANNELS, m_codecachannels);
	DDX_Control(pDX, IDC_CODECA_NAME, m_codecaname);
	DDX_Control(pDX, IDC_CODECA_SAMPLERATE, m_codecasamplerate);
	DDX_Control(pDX, IDC_TASKINFO_INFILE, m_infoinfile);
}


BEGIN_MESSAGE_MAP(MediaInformationDlg, CDialog)
	ON_BN_CLICKED(IDC_TASKINFO_INFILE_FDLG, &MediaInformationDlg::OnBnClickedTaskinfoInfileFdlg)
	ON_BN_CLICKED(IDOK, &MediaInformationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MediaInformationDlg ��Ϣ�������


void MediaInformationDlg::OnBnClickedTaskinfoInfileFdlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePathName;
//	//�ļ������ַ���
	CString strfilter;
	strfilter.Append(_T("Common media formats|*.avi;*.wmv;*.wmp;*.wm;*.asf;*.rm;*.ram;*.rmvb;*.ra;*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;"));
	strfilter.Append(_T("*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.vob;*.ac3;*.dts;*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2;*.swf;*.ogg;*.wma;*.wav;"));
	strfilter.Append(_T("*.mid;*.midi;*.mpa;*.mp2;*.mp3;*.m1a;*.m2a;*.m4a;*.aac;*.mkv;*.ogm;*.m4b;*.tp;*.ts;*.tpr;*.pva;*.pss;*.wv;*.m2ts;*.evo;"));
	strfilter.Append(_T("*.rpm;*.realpix;*.rt;*.smi;*.smil;*.scm;*.aif;*.aiff;*.aifc;*.amr;*.amv;*.au;*.acc;*.dsa;*.dsm;*.dsv;*.dss;*.pmp;*.smk;*.flic|"));
	strfilter.Append(_T("Windows Media Video(*.avi;*wmv;*wmp;*wm;*asf)|*.avi;*.wmv;*.wmp;*.wm;*.asf|"));
	strfilter.Append(_T("Windows Media Audio(*.wma;*wav;*aif;*aifc;*aiff;*mid;*midi;*rmi)|*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi|"));
	strfilter.Append(_T("Real(*.rm;*ram;*rmvb;*rpm;*ra;*rt;*rp;*smi;*smil;*.scm)|*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.rt;*.rp;*.smi;*.smil;*.scm|"));
	strfilter.Append(_T("MPEG Video(*.mpg;*mpeg;*mpe;*m1v;*m2v;*mpv2;*mp2v;*dat;*mp4;*m4v;*m4p;*m4b;*ts;*tp;*tpr;*pva;*pss;*.wv;)|"));
	strfilter.Append(_T("*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.m4b;*.ts;*.tp;*.tpr;*.pva;*.pss;*.wv;|"));
	strfilter.Append(_T("MPEG Audio(*.mpa;*mp2;*m1a;*m2a;*m4a;*aac;*.m2ts;*.evo)|*.mpa;*.mp2;*.m1a;*.m2a;*.m4a;*.aac;*.m2ts;*.evo|"));
	strfilter.Append(_T("DVD(*.vob;*ifo;*ac3;*dts)|*.vob;*.ifo;*.ac3;*.dts|MP3(*.mp3)|*.mp3|CD Tracks(*.cda)|*.cda|"));
	strfilter.Append(_T("Quicktime(*.mov;*qt;*mr;*3gp;*3gpp;*3g2;*3gp2)|*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2|"));
	strfilter.Append(_T("Flash Files(*.flv;*swf;*.f4v)|*.flv;*.swf;*.f4v|Playlist(*.smpl;*.asx;*m3u;*pls;*wvx;*wax;*wmx;*mpcpl)|*.smpl;*.asx;*.m3u;*.pls;*.wvx;*.wax;*.wmx;*.mpcpl|"));
	strfilter.Append(_T("Others(*.ivf;*au;*snd;*ogm;*ogg;*fli;*flc;*flic;*d2v;*mkv;*pmp;*mka;*smk;*bik;*ratdvd;*roq;*drc;*dsm;*dsv;*dsa;*dss;*mpc;*divx;*vp6;*.ape;*.flac;*.tta;*.csf)"));
	strfilter.Append(_T("|*.ivf;*.au;*.snd;*.ogm;*.ogg;*.fli;*.flc;*.flic;*.d2v;*.mkv;*.pmp;*.mka;*.smk;*.bik;*.ratdvd;*.roq;*.drc;*.dsm;*.dsv;*.dsa;*.dss;*.mpc;*.divx;*.vp6;*.ape;*.amr;*.flac;*.tta;*.csf|"));
	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,lpszfilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();
		SetInputPath(FilePathName);
		SetInformation(FilePathName);
	}
}

void MediaInformationDlg::SetInformation(CString filePath)
{
	CString input_protocol,
		input_format,wxh,
		decoder_name,
		decoder_type,
		bitrate,
		extention,
		framerate,
		timelong,
		decoder_name_au,
		sample_rate_au,
		channels_au;

	float framerate_temp,timelong_temp,bitrate_temp;

	AVFormatContext	*pFormatCtx;
	int				i,video_stream = -1,audio_stream = -1;
	AVCodecContext	*pCodecCtx = NULL,*pCodecCtx_au = NULL;
	AVCodec			*pCodec,*pCodec_au;

	//���
	m_codecachannels.SetWindowText(_T(""));
	m_codecaname.SetWindowText(_T(""));
	m_codecasamplerate.SetWindowText(_T(""));
	m_codecvframerate.SetWindowText(_T(""));
	m_codecvname.SetWindowText(_T(""));
	m_codecvresolution.SetWindowText(_T(""));
	m_formatbitrate.SetWindowText(_T(""));
	m_formatduration.SetWindowText(_T(""));
	m_formatinputformat.SetWindowText(_T(""));

	av_register_all();

	pFormatCtx = avformat_alloc_context();


	if(avformat_open_input(&pFormatCtx,filePath,NULL,NULL)!=0){
		AfxMessageBox("�޷���������,������.\n");
	}


	
	if(avformat_find_stream_info(pFormatCtx,NULL)<0)
	{
		AfxMessageBox("�޷��ҵ�����Ϣ��\n"); 
	}

	for(i=0; i<pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			video_stream=i;
		}if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			audio_stream=i;
		}
	}

	
	if(-1 == audio_stream && -1 == video_stream)
	{
		AfxMessageBox("�޷��ҵ���Ƶ������Ƶ����\n");
	}


	if(video_stream!=-1){
		pCodecCtx=pFormatCtx->streams[video_stream]->codec;
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		if(NULL == pCodec)
		{
			AfxMessageBox("û���ҵ���Ƶ���������Ϣ��\n");
		}
		if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
		{
			AfxMessageBox("���ܴ���Ƶ���������\n"); 
		}

		wxh.Format(_T("%d x %d"),pCodecCtx->width,pCodecCtx->height);
		m_codecvresolution.SetWindowText(wxh);

		decoder_name.Format(_T("%s"),pCodecCtx->codec->long_name);

		m_codecvname.SetWindowText(decoder_name);
		
		framerate_temp=(pFormatCtx->streams[video_stream]->r_frame_rate.num)/(pFormatCtx->streams[video_stream]->r_frame_rate.den);
		framerate.Format(_T("%5.2ffps"),framerate_temp);
		m_codecvframerate.SetWindowText(framerate);
		
		/*if(framerate_temp>=500){
			video_stream=-1;
		}*/
	}

	if(audio_stream!=-1){

		pCodecCtx_au=pFormatCtx->streams[audio_stream]->codec;
		pCodec_au=avcodec_find_decoder(pCodecCtx_au->codec_id);
		if(pCodec_au==NULL)
		{
			AfxMessageBox("û���ҵ���Ƶ���������Ϣ��\n");
		}
		if(avcodec_open2(pCodecCtx_au, pCodec_au, NULL)<0)
		{
			AfxMessageBox("���ܴ���Ƶ���������\n"); 
		}

		decoder_name_au.Format(_T("%s"),pCodecCtx_au->codec->long_name);

		m_codecaname.SetWindowText(decoder_name_au);
		sample_rate_au.Format(_T("%d"),pCodecCtx_au->sample_rate);
		m_codecasamplerate.SetWindowText(sample_rate_au);
		channels_au.Format(_T("%d"),pCodecCtx_au->channels);
		m_codecachannels.SetWindowText(channels_au);
	}

	bitrate_temp=((float)(pFormatCtx->bit_rate))/1000;
	bitrate.Format(_T("%5.2fkbps"),bitrate_temp);
	m_formatbitrate.SetWindowText(bitrate);

	timelong_temp=(pFormatCtx->duration)/1000000;

	int tns, thh, tmm, tss;
	tns  = (pFormatCtx->duration)/1000000;
	thh  = tns / 3600;
	tmm  = (tns % 3600) / 60;
	tss  = (tns % 60);
	timelong.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
	m_formatduration.SetWindowText(timelong);

	input_format.Format(_T("%s"),pFormatCtx->iformat->long_name);

	m_formatinputformat.SetWindowText(input_format);


	if(video_stream!=-1){
		avcodec_close(pCodecCtx);
	}
	
	if(audio_stream!=-1){
		avcodec_close(pCodecCtx_au);
	}

	avformat_close_input(&pFormatCtx);
}

void MediaInformationDlg::SetInputPath(CString inputurl){

	m_infoinfile.SetWindowTextA(inputurl);

}

void MediaInformationDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}
