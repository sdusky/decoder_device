#ifndef DECODER_UDPSOCKET_H
#define DECODER_UDPSOCKET_H
#include "decoder_package.h"
#include <QUdpSocket>
/**
 * @brief 信息接收类，继承自QUdpSocket
 *        邢大天 2014-12-06
 *
 */
class decoder_udpsocket : public QUdpSocket
{
    Q_OBJECT
public:

    /**
     * @brief 设置本地端口
     *
     * @param port 本地接收端口
     */
    decoder_udpsocket(int port);

signals:

    /**
     * @brief 信号，发送要解包数据
     *
     * @param data 要解包数据
     */
    void send_data_to_information(QByteArray data);

    /**
     * @brief 信号，发送数据来自的ip
     *
     * @param remoteip 远端ip
     */
    void send_ip_to_information(QString remoteip);
private:
    QByteArray data_from_encoder;   /**< 接收到的数据 */
private slots:

    /**
     * @brief 接收数据
     *
     */
    void get_data_from_encoder();
};

#endif // DECODER_UDPSOCKET_H
