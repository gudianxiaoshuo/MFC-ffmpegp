// TabVedioDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication5.h"
#include "TabVedioDlg.h"
#include "afxdialogex.h"

extern "C"
{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libswscale/swscale.h"
#include "include/SDL2/SDL.h"
};

#define SFM_VEDIO_REFRESH_EVENT  (SDL_USEREVENT + 1)

#define SFM_VEDIO_BREAK_EVENT  (SDL_USEREVENT + 2)

int thread_vedio_exit=0;
int thread_vedio_pause=0;

int sfp_refresh_vedio_thread(void *opaque){

	thread_vedio_exit=0;
	thread_vedio_pause=0;

	while (thread_vedio_exit==0) {
		if(!thread_vedio_pause){
			SDL_Event event;
			event.type = SFM_VEDIO_REFRESH_EVENT;
			SDL_PushEvent(&event);
		}
		SDL_Delay(40);
	}
	//Quit
	SDL_Event event;
	event.type = SFM_VEDIO_BREAK_EVENT;
	SDL_PushEvent(&event);
	thread_vedio_exit=0;
	thread_vedio_pause=0;
	return 0;
}

int ffmpeg_player(LPVOID lpParam)
{

	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int ret, got_picture;

	//------------SDL----------------
	int screen_w,screen_h;
	SDL_Window *screen; 
	SDL_Renderer* sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect;
	SDL_Thread *video_tid;
	SDL_Event event;

	struct SwsContext *img_convert_ctx;
	//===========================================
	//文件路径
	TabVedioDlg *dlg=(TabVedioDlg *)lpParam;
	char filepath[250]={0};
	GetWindowTextA(dlg->m_url,(LPSTR)filepath,250);
	//===========================================

	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		AfxMessageBox("Couldn't open input stream.\n");
		return -1;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		AfxMessageBox("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
		if(videoindex==-1){
			AfxMessageBox("Didn't find a video stream.\n");
			return -1;
		}
		pCodecCtx=pFormatCtx->streams[videoindex]->codec;
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		if(pCodec==NULL){
			AfxMessageBox("Codec not found.\n");
			return -1;
		}
		if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
			AfxMessageBox("Could not open codec.\n");
			return -1;
		}
		pFrame=av_frame_alloc();
		pFrameYUV=av_frame_alloc();
		out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
		avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

		img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
			pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL); 


		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER)) {  
			AfxMessageBox( "Could not initialize SDL\n"); 
			return -1;
		} 
		//SDL 2.0 Support for multiple windows
		screen_w = pCodecCtx->width;
		screen_h = pCodecCtx->height;

		//显示在弹出窗口
		//screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		//	screen_w, screen_h,SDL_WINDOW_OPENGL);
		//===========================================
		//显示在MFC控件上
		screen = SDL_CreateWindowFrom(dlg->GetDlgItem(IDC_SCREEN_VEDIO)->GetSafeHwnd());
		//===========================================
		if(!screen) {  
			AfxMessageBox("SDL: could not create window - exiting\n");
			return -1;
		}
		sdlRenderer = SDL_CreateRenderer(screen, -1, 0);  
		//IYUV: Y + U + V  (3 planes)
		//YV12: Y + V + U  (3 planes)
		sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,pCodecCtx->width,pCodecCtx->height);  

		sdlRect.x=0;
		sdlRect.y=0;
		sdlRect.w=screen_w;
		sdlRect.h=screen_h;

		packet=(AVPacket *)av_malloc(sizeof(AVPacket));

		video_tid = SDL_CreateThread(sfp_refresh_vedio_thread,NULL,NULL);
		//------------SDL End------------
		//Event Loop

		for (;;) {
			//Wait
			SDL_WaitEvent(&event);
			if(event.type==SFM_VEDIO_REFRESH_EVENT){
				//------------------------------
				if(av_read_frame(pFormatCtx, packet)>=0){
					if(packet->stream_index==videoindex){
						ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
						if(ret < 0){
							AfxMessageBox("Decode Error.\n");
							return -1;
						}
						if(got_picture){
							sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
							//SDL---------------------------
							SDL_UpdateTexture( sdlTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0] );  
							SDL_RenderClear( sdlRenderer );  
							//SDL_RenderCopy( sdlRenderer, sdlTexture, &sdlRect, &sdlRect );  
							SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, NULL);  
							SDL_RenderPresent( sdlRenderer );  
							//SDL End-----------------------
							TRACE("Decode 1 frame\n");
						}
					}
					av_free_packet(packet);
				}else{
					//Exit Thread
					thread_vedio_exit=1;
				}
			}else if(event.type==SDL_QUIT){
				thread_vedio_exit=1;
			}else if(event.type==SFM_VEDIO_BREAK_EVENT){
				break;
			}

		}

		sws_freeContext(img_convert_ctx);

		SDL_DestroyWindow(screen);
		SDL_Quit();
		//FIX Small Bug
		//SDL Hide Window When it finished
		dlg->GetDlgItem(IDC_SCREEN_VEDIO)->ShowWindow(SW_SHOWNORMAL);
		//--------------
		av_frame_free(&pFrameYUV);
		av_frame_free(&pFrame);
		avcodec_close(pCodecCtx);
		avformat_close_input(&pFormatCtx);

		return 0;
}

UINT Thread_Vedio__Play(LPVOID lpParam){
	TabVedioDlg *dlg=(TabVedioDlg *)lpParam;
	ffmpeg_player(lpParam);
	return 0;
}

// TabVedioDlg 对话框

IMPLEMENT_DYNAMIC(TabVedioDlg, CDialog)

TabVedioDlg::TabVedioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TabVedioDlg::IDD, pParent)
{

}

TabVedioDlg::~TabVedioDlg()
{
}

void TabVedioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URL, m_url);
}


BEGIN_MESSAGE_MAP(TabVedioDlg, CDialog)

	ON_BN_CLICKED(IDC_FILEBROWSER, &TabVedioDlg::OnBnClickedFilebrowser)
	ON_BN_CLICKED(IDCANCEL, &TabVedioDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_START, &TabVedioDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_PAUSE, &TabVedioDlg::OnBnClickedPause)
	ON_BN_CLICKED(ID_STOP, &TabVedioDlg::OnBnClickedStop)
	ON_STN_CLICKED(IDC_SCREEN_VEDIO, &TabVedioDlg::OnStnClickedScreenVedio)
END_MESSAGE_MAP()

CString TabVedioDlg::URLtoFilename(CString url)
{
	CString filename;
	int nPos=url.ReverseFind('\\');   
	filename=url.Mid(nPos+1);   
	return filename;
}

CString TabVedioDlg::bytetoKBMBGB(int filesize)
{
	//文件大小转换为字符串
	CString filesize_str;
	float filesize_f;
	const int GB = 1024 * 1024 * 1024;//定义GB的计算常量
	const int MB = 1024 * 1024;//定义MB的计算常量
	const int KB = 1024;//定义KB的计算常量

	if (filesize / GB >= 1)
	{//如果当前Byte的值大于等于1GB
		filesize_f=((float)filesize)/((float)GB);
		filesize_str.Format(_T("%.2fGB"),filesize_f);//将其转换成GB
	}
	else if (filesize / MB >= 1)
	{//如果当前Byte的值大于等于1MB
		filesize_f=((float)filesize)/((float)MB);
		filesize_str.Format(_T("%.2fMB"),filesize_f);//将其转换成MB
	}
	else if (filesize / KB >= 1)
	{//如果当前Byte的值大于等于1KB
		filesize_f=((float)filesize)/((float)KB);
		filesize_str.Format(_T("%.2fKB"),filesize_f);//将其转换成KGB
	}
	else
	{
		filesize_f=((float)filesize);
		filesize_str.Format(_T("%.2fB"),filesize_f);//显示Byte值
	}
	return filesize_str;
}





void TabVedioDlg::OnBnClickedFilebrowser()
{
	CString FilePathName;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,NULL);
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();
		m_url.SetWindowText(FilePathName);
	}
}


void TabVedioDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void TabVedioDlg::OnBnClickedStart()
{
	pThreadVedioPlay = AfxBeginThread(Thread_Vedio__Play,this);//开启线程
	// TODO: 在此添加控件通知处理程序代码
}


void TabVedioDlg::OnBnClickedPause()
{
	// TODO: 在此添加控件通知处理程序代码
	thread_vedio_pause=!thread_vedio_pause;
}


void TabVedioDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	thread_vedio_exit=1;
}


void TabVedioDlg::OnStnClickedScreenVedio()
{
	// TODO: 在此添加控件通知处理程序代码
}
