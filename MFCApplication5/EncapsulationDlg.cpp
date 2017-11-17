// EncapsulationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "EncapsulationDlg.h"
#include "afxdialogex.h"

extern "C"
{
#include "include/libavformat/avformat.h"
};

void SerControlEnableFalse(LPVOID lpParam)
{
	EncapsulationDlg *dlg=(EncapsulationDlg *)lpParam;
	dlg->GetDlgItem(IDC_VIDEOINFILEBUTTON)->EnableWindow(false);
	dlg->GetDlgItem(IDC_AUDIOINFILEBUTTON)->EnableWindow(false);
	dlg->GetDlgItem(IDC_OUTPUTFILEBUTTON)->EnableWindow(false);
	dlg->GetDlgItem(ID_Begin)->EnableWindow(false);
	dlg->GetDlgItem(ID_CLOSE)->EnableWindow(false);

	CString resloader;
	resloader.LoadStringA(IDS_ENCAPSULATING);
	dlg->GetDlgItem(ID_Begin)->SetWindowTextA(resloader);

	/*resloader.LoadStringA(IDS_BEGIN_ENCAPSULATION);
	dlg->GetDlgItem(ID_Begin)->SetWindowTextA(resloader);*/

}

void SerControlEnableTrue(LPVOID lpParam)
{
	EncapsulationDlg *dlg=(EncapsulationDlg *)lpParam;
	dlg->GetDlgItem(IDC_VIDEOINFILEBUTTON)->EnableWindow(true);
	dlg->GetDlgItem(IDC_AUDIOINFILEBUTTON)->EnableWindow(true);
	dlg->GetDlgItem(IDC_OUTPUTFILEBUTTON)->EnableWindow(true);
	dlg->GetDlgItem(ID_Begin)->EnableWindow(true);
	dlg->GetDlgItem(ID_CLOSE)->EnableWindow(true);

	CString resloader;
	resloader.LoadStringA(IDS_BEGIN_ENCAPSULATION);
	dlg->GetDlgItem(ID_Begin)->SetWindowTextA(resloader);
}

bool Encapsulate(LPVOID lpParam)
{
	EncapsulationDlg *dlg=(EncapsulationDlg *)lpParam;

	AVOutputFormat *ofmt = NULL;
	//输入对应一个AVFormatContext，输出对应一个AVFormatContext
	//（Input AVFormatContext and Output AVFormatContext）
	AVFormatContext *ifmt_ctx_v = NULL, *ifmt_ctx_a = NULL,*ofmt_ctx = NULL;
	AVPacket pkt;
	int ret, i;
	int videoindex_v=-1,videoindex_out=-1;
	int audioindex_a=-1,audioindex_out=-1;
	int frame_index=0;
	int64_t cur_pts_v=0,cur_pts_a=0;

	//char *in_filename_v = "cuc_ieschool.ts";
	//char *in_filename_v = "cuc_ieschool.h264";
	//char *in_filename_a = "cuc_ieschool.mp3";
	//char *in_filename_a = "gowest.m4a";
	//char *in_filename_a = "gowest.aac";

	//char *in_filename_v = "lalala.hevc";
	//char *in_filename_a = "huoyuanjia.mp3";
	//char *out_filename = "cuc_ieschool.mp4";
	CString in_filename_v;
	CString in_filename_a;
	CString out_filename;
	dlg->GetDlgItem(IDC_VIDEOINFILE)->GetWindowTextA(in_filename_v);
	dlg->GetDlgItem(IDC_AUDIOINFILE)->GetWindowTextA(in_filename_a);
	dlg->GetDlgItem(IDC_OUTPUTFILE)->GetWindowTextA(out_filename);

	av_register_all();
	
	if ((ret = avformat_open_input(&ifmt_ctx_v, in_filename_v, 0, 0)) < 0)
	{
		AfxMessageBox("无法打开视频输入流,请重试.\n");
		return 0;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx_v, 0)) < 0)
	{
		AfxMessageBox("无法找到视频流信息。\n");  
        return 0;
	}


	if ((ret = avformat_open_input(&ifmt_ctx_a, in_filename_a, 0, 0)) < 0)
	{
		AfxMessageBox("无法打开音频输入文件。\n");  
        return 0;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0) 
	{
		AfxMessageBox("无法找到音频流信息。\n");  
        return 0;
	}

	//输出（Output）
	avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);

	if (!ofmt_ctx)
	{
		AfxMessageBox("无法创建输出文件。\n");
		return 0;
	}

	ofmt = ofmt_ctx->oformat;

	for (i = 0; i < ifmt_ctx_v->nb_streams; i++) 
	{
		//根据输入流创建输出流（Create output AVStream according to input AVStream）
		if(ifmt_ctx_v->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			AVStream *in_stream = ifmt_ctx_v->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
			videoindex_v=i;

			if (!out_stream)
			{
				AfxMessageBox("分配视频输出流失败。\n");
				return 0;
			}

			videoindex_out=out_stream->index;
			//复制AVCodecContext的设置（Copy the settings of AVCodecContext）
			if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) 
			{
				AfxMessageBox( "复制视频流上下文信息失败。\n");
				return 0;
			}

			out_stream->codec->codec_tag = 0;
			if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			{
				out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			}

			break;
		}
	}

	for (i = 0; i < ifmt_ctx_a->nb_streams; i++)
	{
		//根据输入流创建输出流（Create output AVStream according to input AVStream）
		if(ifmt_ctx_a->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO)
		{
			AVStream *in_stream = ifmt_ctx_a->streams[i];
			AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
			audioindex_a=i;
			if (!out_stream) 
			{
				AfxMessageBox( "分配音频输出流失败\n");
				return 0;
			}

			audioindex_out=out_stream->index;
			//复制AVCodecContext的设置（Copy the settings of AVCodecContext）
			if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) 
			{
				AfxMessageBox( "复制音频流上下文信息失败。\n");
				return 0;
			}

			out_stream->codec->codec_tag = 0;
			if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
			{
				out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
			}

			break;
		}
	}

	//打开输出文件（Open output file）
	if (!(ofmt->flags & AVFMT_NOFILE)) 
	{
		if (avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE) < 0) 
		{
			AfxMessageBox( "不能打开文件夹： '"+out_filename+ "'！\n");
			return 0;
		}
	}
	//写文件头（Write file header）
	if (avformat_write_header(ofmt_ctx, NULL) < 0) 
	{
		AfxMessageBox( "写输出文件头失败！\n");
		return 0;
	}

	while (1) 
	{
		AVFormatContext *ifmt_ctx;
		int stream_index=0;
		AVStream *in_stream, *out_stream;

		//获取一个AVPacket（Get an AVPacket）
		if(av_compare_ts(cur_pts_v,
						 ifmt_ctx_v->streams[videoindex_v]->time_base,
						 cur_pts_a,
						 ifmt_ctx_a->streams[audioindex_a]->time_base
						 ) <= 0)
		{
			ifmt_ctx=ifmt_ctx_v;
			stream_index=videoindex_out;

			if(av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do
				{
					in_stream  = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if(pkt.stream_index==videoindex_v)
					{
						//FIX：No PTS 
						//Simple Write PTS
						if(pkt.pts==AV_NOPTS_VALUE)
						{
							//Write PTS
							AVRational time_base1=in_stream->time_base;
							//Duration between 2 frames (us)
							int64_t calc_duration=(double)AV_TIME_BASE/av_q2d(in_stream->r_frame_rate);
							//Parameters
							pkt.pts=(double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							pkt.dts=pkt.pts;
							pkt.duration=(double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							frame_index++;
						}

						cur_pts_v=pkt.pts;
						break;
					}
				}while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			}
			else
			{
				break;
			}
		}
		else
		{
			ifmt_ctx=ifmt_ctx_a;
			stream_index=audioindex_out;
			if(av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do
				{
					in_stream  = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if(pkt.stream_index==audioindex_a)
					{

						//FIX：No PTS
						//Simple Write PTS
						if(pkt.pts==AV_NOPTS_VALUE)
						{
							//Write PTS
							AVRational time_base1=in_stream->time_base;
							//Duration between 2 frames (us)
							int64_t calc_duration=(double)AV_TIME_BASE/av_q2d(in_stream->r_frame_rate);
							//Parameters
							pkt.pts=(double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							pkt.dts=pkt.pts;
							pkt.duration=(double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							frame_index++;
						}
						cur_pts_a=pkt.pts;

						break;
					}
				}while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			}
			else
			{
				break;
			}

		}

		/* copy packet */
		//转换PTS/DTS（Convert PTS/DTS）
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		pkt.stream_index=stream_index;

		//printf("Write 1 Packet. size:%5d\tpts:%8d\n",pkt.size,pkt.pts);
		//写入（Write）
		if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) 
		{
			printf( "Error muxing packet\n");
			break;
		}
		av_free_packet(&pkt);

	}
	//写文件尾（Write file trailer）
	av_write_trailer(ofmt_ctx);

	avformat_close_input(&ifmt_ctx_v);
	avformat_close_input(&ifmt_ctx_a);

	/* close output */
	if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
	{
		avio_close(ofmt_ctx->pb);
	}

	avformat_free_context(ofmt_ctx);


	return 1;
}

UINT ThreadEncapsulate(LPVOID lpParam){

	
	SerControlEnableFalse(lpParam);

	if(Encapsulate(lpParam))
	{
		AfxMessageBox("封装成功！请到相应文件夹下查看!\n");
	}

	SerControlEnableTrue(lpParam);

	return 1;
}
// EncapsulationDlg 对话框

IMPLEMENT_DYNAMIC(EncapsulationDlg, CDialogEx)

EncapsulationDlg::EncapsulationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(EncapsulationDlg::IDD, pParent)
{

}

EncapsulationDlg::~EncapsulationDlg()
{
}

void EncapsulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VIDEOINFILE, m_vedioinfile);
	DDX_Control(pDX, IDC_AUDIOINFILE, m_audioinfile);
	DDX_Control(pDX, IDC_OUTPUTFILE, m_outputfile);
	DDX_Control(pDX, IDC_ENCAPSULATION_MESSAGE, m_encapsulationmessage);
}


BEGIN_MESSAGE_MAP(EncapsulationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_VIDEOINFILEBUTTON, &EncapsulationDlg::OnBnClickedVideoinfilebutton)
	ON_BN_CLICKED(IDC_AUDIOINFILEBUTTON, &EncapsulationDlg::OnBnClickedAudioinfilebutton)
	ON_BN_CLICKED(IDC_OUTPUTFILEBUTTON, &EncapsulationDlg::OnBnClickedOutputfilebutton)
	ON_BN_CLICKED(ID_Begin, &EncapsulationDlg::OnBnClickedBegin)
	ON_BN_CLICKED(ID_CLOSE, &EncapsulationDlg::OnBnClickedClose)
END_MESSAGE_MAP()


// EncapsulationDlg 消息处理程序


BOOL EncapsulationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//IDS_ENCAPSULATION_BRIEF
	CString resloader;
	resloader.LoadStringA(IDS_ENCAPSULATION_BRIEF);

	m_encapsulationmessage.SetWindowTextA(resloader);

	return TRUE;  

}


void EncapsulationDlg::OnBnClickedVideoinfilebutton()
{

	CString FilePathName;
	CString strfilter;
	strfilter.Append(_T("封装格式文件|*.mp4;*.ts;|"));
	strfilter.Append(_T("编码格式视频文件|*.h264;*.hevc;|"));

	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,lpszfilter);
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();

		SetInputVideoPath(FilePathName);

	}
}

void EncapsulationDlg::SetInputVideoPath(CString inputpath)
{
	m_vedioinfile.SetWindowTextA(inputpath);
}

void EncapsulationDlg::OnBnClickedAudioinfilebutton()
{

	CString FilePathName;
	CString strfilter;
	strfilter.Append(_T(".mp3|*.mp3;|"));

	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,lpszfilter);
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();

		SetInputAudioPath(FilePathName);

	}
}

void EncapsulationDlg::SetInputAudioPath(CString inputpath)
{
	m_audioinfile.SetWindowTextA(inputpath);
}

void EncapsulationDlg::OnBnClickedOutputfilebutton()
{
	
	CString FilePathName;
	CString strfilter;
	strfilter.Append(_T(".mp4|*.mp4;|"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(FALSE,NULL,NULL,NULL,lpszfilter);
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName()+ ".mp4";

		SetOutputMediaPath(FilePathName);

	}
}

void EncapsulationDlg::SetOutputMediaPath(CString outputpath)
{
	m_outputfile.SetWindowTextA(outputpath);
}

bool EncapsulationDlg::CheckEncapsulationMessage()
{
	CString vedioInfile;
	CString audioInfile;
	CString outputPath;

	m_vedioinfile.GetWindowTextA(vedioInfile);
	m_audioinfile.GetWindowTextA(audioInfile);
	m_outputfile.GetWindowTextA(outputPath);

	if(vedioInfile.IsEmpty())
	{
		AfxMessageBox("视频文件输入路径不能为空！\n");
		return false;
	}

	if(audioInfile.IsEmpty())
	{
		AfxMessageBox("音频文件输入路径不能为空！\n");
		return false;
	}

	if(outputPath.IsEmpty())
	{
		AfxMessageBox("输出路径不能为空！\n");
		return false;
	}

	return true;
}

void EncapsulationDlg::OnBnClickedBegin()
{
	
	if(!CheckEncapsulationMessage())
	{
		return;
	}

	pThreadEncapsulate = AfxBeginThread(ThreadEncapsulate,this);
}


void EncapsulationDlg::OnBnClickedClose()
{
	CDialog::OnOK();
}
