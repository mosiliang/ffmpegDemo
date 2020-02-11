#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void savePicture(AVFrame *frame, int width, int height, int index);

public slots:
	void on_saveFrame_clicked();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
