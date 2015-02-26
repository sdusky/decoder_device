#ifndef DECODER_INFORMATION_PARSE_H
#define DECODER_INFORMATION_PARSE_H

#include <QObject>
#include "decoder_package.h"

/**
 * @brief 信息解包类，将结构化数据分解
 *        邢大天 2014-12-07
 *        邢大天 2015-02-06
 *
 */
class decoder_information_parse : public QObject
{
    Q_OBJECT
public:
    explicit decoder_information_parse(QObject *parent = 0);

signals:

    /**
     * @brief 信号,将视频数据送解码器
     *
     * @param buffer 视频信息指针
     * @param length 视频信息长度
     */
    void send_video_to_decoder(void *buffer,int length);


    /**
     * @brief 信号，提示显示当前传感器值
     *
     * @param sensor_num 数据中传感器数量
     */
    void show_sensor_data(int sensor_num);

    /**
     * @brief 信号，显示编码器IP
     *
     * @param remoteIp 编码器IP
     */
    void show_system_data(QString remoteIp);
public slots:

    /**
     * @brief 槽函数，获取接收到的UDP的数据
     *格式：
     *          6字节包长度+1个字节传感器数量+6字节视频长度+视频信息+4字节传感器信息长度+传感器信息
     * @param original_data 原始数据
     */
    void get_data_from_udpsocket(QByteArray original_data);

    /**
     * @brief 获取编码器ip
     *
     * @param remoteip 编码器ip
     */
    void get_remoteip_from_udpsocket(QString remoteip);
};

#endif // DECODER_INFORMATION_PARSE_H
