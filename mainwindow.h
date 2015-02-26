#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "decoder_package.h"
#include <QImage>
#include <QTimer>
#include <QPainter>

namespace Ui {
class MainWindow;
}

/**
 * @brief UI显示
 *
 */
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:

    /**
     * @brief 槽函数，显示视频信息
     *
     * @param rgb           rgb指针
     * @param width         图像宽度
     * @param height        图像高度
     */
    void show_video(unsigned char *rgb,int width,int height);

    /**
     * @brief 槽函数，显示传感器信息
     *
     * @param sensor_num 传感器数量
     */
    void show_sensor(int sensor_num);

    /**
     * @brief 槽函数，显示本地信息
     *
     */
    void show_local_information();

    /**
     * @brief 显示黑屏
     *
     */
    void show_blank();

    /**
     * @brief 显示系统信息，解码器ip
     *
     * @param remoteip 解码器ip
     */
    void show_system_data(QString remoteip);
private:
    Ui::MainWindow *ui;
    QTimer* local_timer; /**< 刷新本机状态定时器 */
};

#endif // MAINWINDOW_H
