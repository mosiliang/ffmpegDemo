#include "mainwindow.h"
extern "C" 
{
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
	#include <libavdevice/avdevice.h>
	#include <libavformat/version.h>
	#include <libavutil/time.h>
	#include <libavutil/mathematics.h>
}
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qDebug() << "This is ffmpeg environment create 2020-2-10 21:18";
	qDebug() << avcodec_version();
    MainWindow w;
    w.show();

    return a.exec();
}
