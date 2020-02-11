#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_saveFrame_clicked()
{
	char videoFilePath[] = "D:\\work\\ffmpegDemo\\demo.mp4";
	AVFormatContext *formatCtx;
	AVCodecContext *codecCtx;
	AVCodec *codec;
	AVFrame *frame, *frameRGB;
	AVPacket *packet;
	uint8_t *outBuffer;
	static struct SwsContext *imgConvertCtx;
	int videoStream, i, numBytes;
	int ret, gotPicture;
	av_register_all();//��ʼ��FFMPEG
	formatCtx = avformat_alloc_context();//FFMPEG���ж���ͨ���������
	if (avformat_open_input(&formatCtx, videoFilePath, NULL, NULL) != 0)//����Ƶ
	{
		qDebug() << "WRONG: Video not found.";
		return;
	}
	//�����ļ��а�������Ƶ��������Ϣ����¼���浽videoStream������
	for (int i = 0; i < formatCtx->nb_streams; ++i)
	{
		if (formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
		}
	}
	//δ�ҵ���Ƶ��
	if (videoStream == -1)
	{
		qDebug() << "WRONG: Video stream not found.";
	}
	else 
	{
		//���ұ���������������ʵ���õ���ʲô������
		codecCtx = formatCtx->streams[videoStream]->codec;
		codec = avcodec_find_decoder(codecCtx->codec_id);
		if (codec == NULL)
		{
			qDebug() << "WRONG: Codec not found.";
		}
		else if(avcodec_open2(codecCtx,codec,NULL)<0)
		{
			qDebug() << "WRONG: CodeC can not open.";
		}
		else
		{
			//��ȡ��Ƶ
			int ySize = codecCtx->width*codecCtx->height;
			packet = (AVPacket*)malloc(sizeof(AVPacket));//����һ��packet
			av_new_packet(packet, ySize);//����packet����
			
			frame = av_frame_alloc();
			frameRGB = av_frame_alloc();
			imgConvertCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt, codecCtx->width, codecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
			numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height);
			outBuffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
			avpicture_fill((AVPicture*)frameRGB, outBuffer, AV_PIX_FMT_RGB24, codecCtx->width, codecCtx->height);
			
			av_dump_format(formatCtx, 0, videoFilePath, 0);//�����Ƶ��Ϣ
			int index = 0;
			while (index < 30)//ֻ����30��
			{
				if (av_read_frame(formatCtx, packet) < 0)
				{
					qDebug() << "Have read video.";
					break;
				}
				if (packet->stream_index == videoStream)
				{
					ret = avcodec_decode_video2(codecCtx, frame, &gotPicture, packet);
					if (ret < 0)
					{
						qDebug() << "WRONG: Decode error.";
						return;
					}
					if (gotPicture)
					{
						sws_scale(imgConvertCtx, (uint8_t const * const *)frame->data, frame->linesize, 0, codecCtx->height, frameRGB->data, frameRGB->linesize);
						savePicture(frameRGB, codecCtx->width, codecCtx->height, index++);
					}
				}
				av_free_packet(packet);
			}
			av_free(outBuffer);
			av_free(frameRGB);
			avcodec_close(codecCtx);
			avformat_close_input(&formatCtx);
			QMessageBox::information(NULL, "result", "Suc.");
		}
	}
}

void MainWindow::savePicture(AVFrame *frame, int width, int height, int index)
{
	//��RGB��Ϣ���嵽PPM�ļ���
	FILE *file;
	char fileName[32];
	sprintf(fileName, "frame%d.ppm", index);
	file = fopen(fileName, "wb");
	if (file == NULL)
	{
		return;
	}
	//header
	fprintf(file, "P6\n%d\n%d\n255", width, width);
	//pixel data
	for (int y = 0; y < height; ++y)
	{
		fwrite(frame->data[0] + y*frame->linesize[0], 1, width * 3, file);
	}
	fclose(file);
}
