// TabRecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "TabRecordDlg.h"
#include "afxdialogex.h"

//#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libswscale/swscale.h"
#include "include/libavdevice/avdevice.h"
#include "include/libavutil/audio_fifo.h"
#include "include/SDL2/SDL.h"

};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* sTargerTitle = "desktop";


//Refresh Event
#define SFM_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_exit=0;

int sfp_refresh_thread(void *opaque){

	thread_exit=0;

	while (thread_exit==0) {
		
		SDL_Event event;
		event.type = SFM_REFRESH_EVENT;
		SDL_PushEvent(&event);
		
		SDL_Delay(10);
	}
	//Quit
	SDL_FlushEvent(SFM_REFRESH_EVENT);
	SDL_Event event;
	event.type = SFM_BREAK_EVENT;
	SDL_PushEvent(&event);
	thread_exit=0;
	
	return 0;
}


int my_ffmpeg_player(LPVOID lpParam)
{
	 
	//TabRecordDlg *dlg=(TabRecordDlg *)this;
	TabRecordDlg *dlg=(TabRecordDlg *)lpParam;
	AVFormatContext *pAVFormatContext;
	unsigned int    i;
    int             streamindex;  
    AVCodecContext  *pAVCodecContext;  
    AVCodec         *pAVCodec; 
	AVPacket        *pAVPacket;

	av_register_all(); 
	pAVFormatContext = avformat_alloc_context();
	avdevice_register_all(); 

	AVDictionary* pAVDictionary = NULL; 
	//av_dict_set(&pAVDictionary, "framerate", "15", NULL);
	AVInputFormat *ifmt=av_find_input_format("gdigrab");
	
	if(avformat_open_input(&pAVFormatContext,sTargerTitle,ifmt,&pAVDictionary) != 0)
	{
		AfxMessageBox("无法打开输入流,请重试.\n");
		return 0;
	}

	if(avformat_find_stream_info(pAVFormatContext,NULL)<0)  
    {  
        AfxMessageBox("无法找到流信息。\n");  
        return 0;
    } 

	streamindex = -1;
	for(i = 0; i<pAVFormatContext->nb_streams; i++)
	{
		if(pAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			streamindex = i;
			break;
		}
	}

	if(-1 == streamindex)
	{
		AfxMessageBox("无法找到视频流。\n");
		return 0;
	}

	pAVCodecContext = pAVFormatContext->streams[streamindex]->codec;
	pAVCodec = avcodec_find_decoder(pAVCodecContext->codec_id);

	if(NULL == pAVCodec)
	{
		AfxMessageBox("没有找到编解码器信息。\n");
		return 0;
	}
	if(avcodec_open2(pAVCodecContext, pAVCodec,NULL)<0)  
    {  
		AfxMessageBox("不能打开编解码器。\n");  
        return 0;  
    } 
	AVFrame *pFrame,*pFrameYUV;  
	uint8_t *out_buffer;
    pFrame=av_frame_alloc();  
    pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pAVCodecContext->width, pAVCodecContext->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pAVCodecContext->width, pAVCodecContext->height);


	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {    
        AfxMessageBox("不能初始化SDL。\n");  
        return 0;  
    } 

	int screen_w,screen_h;
	SDL_Window *pSDL_Window; 
	SDL_Renderer* pSDL_Renderer;
	SDL_Texture* pSDL_Texture;
	SDL_Rect sdlRect;
	SDL_Thread *pSDL_Thread;
	SDL_Event sdl_Event;

	struct SwsContext *img_convert_ctx;
	img_convert_ctx = sws_getContext(pAVCodecContext->width, pAVCodecContext->height, pAVCodecContext->pix_fmt, 
			pAVCodecContext->width, pAVCodecContext->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
	 

	CRect rectTemp;
	dlg->GetDlgItem(IDC_SCREENWINDOW)->GetWindowRect(rectTemp);
	screen_w = rectTemp.Width();
	screen_h = rectTemp.Height();

	pSDL_Window = SDL_CreateWindowFrom(dlg->GetDlgItem(IDC_SCREENWINDOW)->GetSafeHwnd());
	if(NULL == pSDL_Window) 
	{  
		AfxMessageBox("SDL: 无法创建SDL_WINDOW。\n");
		return 0;
	}
	/*SDL_CreateRenderer(window,index,flags)
	window	： 渲染的目标窗口。

	index	：打算初始化的渲染设备的索引。设置“-1”则初始化默认的渲染设备。

	flags	：支持以下值（位于SDL_RendererFlags定义中）
		SDL_RENDERER_SOFTWARE ：使用软件渲染
	
		SDL_RENDERER_ACCELERATED ：使用硬件加速
		SDL_RENDERER_PRESENTVSYNC：和显示器的刷新率同步

		SDL_RENDERER_TARGETTEXTURE ：不太懂

	返回创建完成的渲染器的ID。如果创建失败则返回NULL。
	*/
	pSDL_Renderer = SDL_CreateRenderer(pSDL_Window, -1, 0);

		//IYUV: Y + U + V  (3 planes)
		//YV12: Y + V + U  (3 planes)
	pSDL_Texture = SDL_CreateTexture(pSDL_Renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pAVCodecContext->width,pAVCodecContext->height);  

	sdlRect.x=0;
	sdlRect.y=0;
	sdlRect.w=screen_w;
	sdlRect.h=screen_h;


	pAVPacket=(AVPacket *)av_malloc(sizeof(AVPacket));

	CString outFile;
	outFile.Format("output(%d_%d).yuv",pAVCodecContext->width,pAVCodecContext->height);

	FILE *fp_yuv = NULL;
	fopen_s(&fp_yuv,outFile, "wb+");
	if(NULL == fp_yuv)
	{
		AfxMessageBox("无法创建文件。\n");
		return 0;
	}
	int got_picture_ptr;

	pSDL_Thread = SDL_CreateThread(sfp_refresh_thread,NULL,NULL);
	

	for (;;) 
	{
			//Wait
		SDL_WaitEvent(&sdl_Event);
		if(sdl_Event.type==SFM_REFRESH_EVENT)
		{
			if(av_read_frame(pAVFormatContext, pAVPacket) >= 0)
			{
				if(pAVPacket->stream_index == streamindex)
				{
					if(avcodec_decode_video2(pAVCodecContext, pFrame, &got_picture_ptr, pAVPacket) < 0)
					{
						AfxMessageBox("解码一帧视频数据失败。\n");
						return 0;
					}
					if(got_picture_ptr)
					{
						sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pAVCodecContext->height, pFrameYUV->data, pFrameYUV->linesize);
						
						SDL_UpdateTexture(pSDL_Texture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);  
						SDL_RenderClear(pSDL_Renderer);  
						SDL_RenderCopy(pSDL_Renderer, pSDL_Texture, NULL,  &sdlRect);  
						SDL_RenderPresent(pSDL_Renderer);  



						int y_size = pAVCodecContext->width * pAVCodecContext->height;      
                        fwrite(pFrameYUV->data[0],1,y_size,fp_yuv);    //Y     
                        fwrite(pFrameYUV->data[1],1,y_size/4,fp_yuv);  //U    
                        fwrite(pFrameYUV->data[2],1,y_size/4,fp_yuv);  //V   
					}//if(got_picture_ptr)
				}//if(pAVPacket->stream_index == streamindex)
				av_free_packet(pAVPacket);
			}//if(av_read_frame(pAVFormatContext, pAVPacket) >= 0)
			else
			{
				//播放完毕
				//Exit Thread
				thread_exit=1;
			}
		}//if(sdl_Event.type==SFM_REFRESH_EVENT)
		else if(sdl_Event.type==SDL_QUIT)
		{
			thread_exit=1;
		}
		else if(sdl_Event.type==SFM_BREAK_EVENT)
		{
			break;
		}
	}//for

	char pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory( MAX_PATH, pFileName);
	CString csFullPath(pFileName); 
	dlg->GetDlgItem(IDC_YUVFILE)->SetWindowTextA(csFullPath + "\\" + outFile);

	sws_freeContext(img_convert_ctx);
	fclose(fp_yuv); 
	SDL_DestroyWindow(pSDL_Window);
	SDL_Quit();
	dlg->GetDlgItem(IDC_SCREENWINDOW)->ShowWindow(SW_SHOWNORMAL);
	av_free(pFrameYUV);  
	avcodec_close(pAVCodecContext);  
	avformat_close_input(&pAVFormatContext);
	return 1;
}

UINT Thread_Play(LPVOID lpParam){
	TabRecordDlg *dlg=(TabRecordDlg *)lpParam;
	my_ffmpeg_player(lpParam);
	
	dlg->GetDlgItem(IDOK)->EnableWindow(true);
	dlg->GetDlgItem(IDC_ENDRECORD)->EnableWindow(false);
	dlg->GetDlgItem(IDC_ENCODE)->EnableWindow(true);

	return 0;
}

int flush_encoder(AVFormatContext *fmt_ctx,unsigned int stream_index){
	int ret;
	int got_frame;
	AVPacket enc_pkt;
	if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &CODEC_CAP_DELAY))
	{
		return 0;
	}
	while (1)
	{
		enc_pkt.data = NULL;
		enc_pkt.size = 0;
		av_init_packet(&enc_pkt);
		ret = avcodec_encode_video2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,NULL, &got_frame);
		av_frame_free(NULL);
		if (ret < 0)
			break;
		if (!got_frame)
		{
			ret=0;
			break;
		}

		ret = av_write_frame(fmt_ctx, &enc_pkt);
		if (ret < 0)
		{
			break;
		}
	}
	return ret;
}

int save_record(CString outfilepath, CString infilepath)
{
	AVFormatContext* pFormatCtx;
	AVOutputFormat* fmt;
	AVStream* video_st;
	AVCodecContext* pCodecCtx;
	AVCodec* pCodec;
	AVPacket pkt;
	uint8_t* picture_buf;
	AVFrame* pFrame;
	int picture_size;
	int y_size;
	int framecnt=0;
	int framenum=1000;									//Frames to encode

	const char* out_file = outfilepath;
	FILE *in_file = NULL;
		

	fopen_s(&in_file, infilepath, "rb");


	if(NULL == in_file)
	{
		AfxMessageBox("无法打开文件。\n");
		return 0;
	}
	int nPos = infilepath.Find('\\');
	CString sSubStr = infilepath;
	while(nPos)
	{
		sSubStr = sSubStr.Mid(nPos + 1,sSubStr.GetLength() - nPos);
		nPos = sSubStr.Find('\\');
		if(nPos == -1)
		{
			nPos = 0;
		}
	}
	int nstrPos = sSubStr.Find(_T("("));
	int nmidPos = sSubStr.Find(_T("_"));
    int nendPos = sSubStr.Find(_T(")"));
	int in_w = _ttoi(sSubStr.Mid(nstrPos + 1,nmidPos));
	int in_h = _ttoi(sSubStr.Mid(nmidPos + 1,nendPos));

	av_register_all();

	pFormatCtx = avformat_alloc_context();

	fmt = av_guess_format(NULL, out_file, NULL);
	pFormatCtx->oformat = fmt;

	if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		AfxMessageBox("输出文件打开失败！\n");
		return 0;
	}

	video_st = avformat_new_stream(pFormatCtx, 0);
	video_st->time_base.num = 1; 
	video_st->time_base.den = 25;  

	pCodecCtx = video_st->codec;
	pCodecCtx->codec_id = fmt->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = PIX_FMT_YUV420P;
	pCodecCtx->width = in_w;  
	pCodecCtx->height = in_h;
	pCodecCtx->time_base.num = 1;  
	pCodecCtx->time_base.den = 25;  
	pCodecCtx->bit_rate = 400000;  
	pCodecCtx->gop_size=250;// 图像组大小
	pCodecCtx->qmin = 10;// 最小视频量化标度
	pCodecCtx->qmax = 51;// 最大视频量化标度
	pCodecCtx->max_b_frames=3;// 最大B帧数

	AVDictionary *param = 0;
	//H.264
	if(pCodecCtx->codec_id == AV_CODEC_ID_H264) {
		av_dict_set(&param, "preset", "slow", 0);
		av_dict_set(&param, "tune", "zerolatency", 0);
	}
	//H.265
	if(pCodecCtx->codec_id == AV_CODEC_ID_H265){
		av_dict_set(&param, "preset", "ultrafast", 0);
		av_dict_set(&param, "tune", "zero-latency", 0);
	}

	av_dump_format(pFormatCtx, 0, out_file, 1);

	pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec){
		AfxMessageBox("Can not find encoder! (没有找到合适的编码器！)\n");
		return 0;
	}
	if (avcodec_open2(pCodecCtx, pCodec,&param) < 0){
		AfxMessageBox("Failed to open encoder! (编码器打开失败！)\n");
		return 0;
	}


	pFrame = av_frame_alloc();
	picture_size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
	picture_buf = (uint8_t *)av_malloc(picture_size);
	avpicture_fill((AVPicture *)pFrame, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);

	//Write File Header
	avformat_write_header(pFormatCtx,NULL);

	av_new_packet(&pkt,picture_size);

	y_size = pCodecCtx->width * pCodecCtx->height;

	for (int i=0; i<framenum; i++){
		//Read raw YUV data
		if (fread(picture_buf, 1, y_size*3/2, in_file) < 0){
			AfxMessageBox("Failed to read raw data! (文件读取错误！)\n");
			return 0;
		}else if(feof(in_file)){
			break;
		}
		pFrame->data[0] = picture_buf;              // Y
		pFrame->data[1] = picture_buf+ y_size;      // U 
		pFrame->data[2] = picture_buf+ y_size*5/4;  // V
		pFrame->pts=i;
		int got_picture=0;
		//Encode
		int ret = avcodec_encode_video2(pCodecCtx, &pkt,pFrame, &got_picture);
		if(ret < 0){
			AfxMessageBox("编码失败！ \n");
			return 0;
		}
		if (got_picture==1){
			
			framecnt++;
			pkt.stream_index = video_st->index;
			ret = av_write_frame(pFormatCtx, &pkt);
			av_free_packet(&pkt);
		}
	}
	//Flush Encoder
	int ret = flush_encoder(pFormatCtx,0);
	if (ret < 0) {
		AfxMessageBox("输出剩余AVPacket失败\n");
		return 0;
	}

	//Write file trailer
	av_write_trailer(pFormatCtx);

	//Clean
	if (video_st){
		avcodec_close(video_st->codec);
		av_free(pFrame);
		av_free(picture_buf);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);

	fclose(in_file);
	AfxMessageBox("保存成功，请到相应文件夹下查看！\n");
	return 1;
}

UINT Thread_Save(LPVOID lpParam){
	CString resloader;
	TabRecordDlg *dlg=(TabRecordDlg *)lpParam;

	CString inYUVFilePath; 
	CString outFilePathName;
	//文件过滤字符串
	CString strfilter;
	dlg->GetDlgItem(IDC_YUVFILE)->GetWindowTextA(inYUVFilePath);
	strfilter.Append(_T("hevc(*.hevc)|*.hevc|"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog cfdlg(FALSE,NULL,NULL,NULL,lpszfilter);///TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	if(cfdlg.DoModal()==IDOK) {
		outFilePathName=cfdlg.GetPathName() + ".hevc";
		dlg->GetDlgItem(IDOK)->EnableWindow(false);
		dlg->GetDlgItem(IDC_ENCODE)->EnableWindow(false);
		resloader.LoadStringA(IDS_ENCODEING);
		dlg->GetDlgItem(IDC_ENCODE)->SetWindowTextA(resloader);
		save_record(outFilePathName,inYUVFilePath);

		dlg->GetDlgItem(IDOK)->EnableWindow(true);
		dlg->GetDlgItem(IDC_ENCODE)->EnableWindow(true);
		resloader.LoadStringA(IDS_ENCODE_SAVE);
		dlg->GetDlgItem(IDC_ENCODE)->SetWindowTextA("编码保存");
	}

	return 0;
}






// TabRecordDlg 对话框

IMPLEMENT_DYNAMIC(TabRecordDlg, CDialog)

TabRecordDlg::TabRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TabRecordDlg::IDD, pParent)
{

}

TabRecordDlg::~TabRecordDlg()
{
}

void TabRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YUVFILE, m_yuvoutfile);
	DDX_Control(pDX, IDC_ENCODE, m_savefile);
	DDX_Control(pDX, IDC_ENDRECORD, m_stoprecord);
	DDX_Control(pDX, IDOK, m_beginrecord);
}


BEGIN_MESSAGE_MAP(TabRecordDlg, CDialog)
	ON_BN_CLICKED(IDOK, &TabRecordDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ENDRECORD, &TabRecordDlg::OnBnClickedEndrecord)
	ON_BN_CLICKED(IDC_ENCODE, &TabRecordDlg::OnBnClickedEncode)
END_MESSAGE_MAP()


// TabRecordDlg 消息处理程序


void TabRecordDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_beginrecord.EnableWindow(false);
	m_stoprecord.EnableWindow(true);

	pThreadPlay = AfxBeginThread(Thread_Play,this);
	//CDialog::OnOK();
}


void TabRecordDlg::OnBnClickedEndrecord()
{
	thread_exit=1;// TODO: 在此添加控件通知处理程序代码
}



void TabRecordDlg::OnBnClickedEncode()
{
	// TODO: 在此添加控件通知处理程序代码
	pThreadSave = AfxBeginThread(Thread_Save,this);
	
}
