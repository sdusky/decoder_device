#ifndef DECODER_TCPSOCKET_H
#define DECODER_TCPSOCKET_H
#include "decoder_package.h"
#include <QTcpSocket>


/**
 * @brief 服务器连接类，继承自QTcpSocket
 *        邢大天 2014-12-06
 *
 */
class decoder_tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:

    /**
     * @brief 构造函数，设置服务器ip和服务器端口
     *
     * @param hostip 服务器ip
     * @param hostport 服务器port
     */
    decoder_tcpsocket(QString hostip,int hostport);
    ~decoder_tcpsocket()
    {
        ;
    }

signals:

    /**
     * @brief 信号，发送错误
     *
     * @param error_no 错误信息
     */
    void connect_error(int error_no);
private:

    /**
     * @brief 向服务器返回操作结果
     *
     * @param op_code 操作码
     * @param op_data 操作数
     */
    void send_info_to_host(QString op_code,QString op_data);

    /**
     * @brief 解析数据
     *
     * @param data 要解析的数据
     */
    void data_parse(QByteArray data);

    /**
     * @brief 设置设备状态
     *
     * @param state_num 状态号IDLE / BUSY
     */
    void change_device_state(int state_num);

    QByteArray data_from_host;  /**< 来自服务器的数据 */
    QByteArray data_to_host;    /**< 向服务器的数据 */
    QString host_ip;            /**< 服务器ip */
    int     host_port;          /**< 服务器端口 */

private slots:

    /**
     * @brief 接收服务器数据
     *
     */
    void get_data_from_host();

    /**
     * @brief 连接到服务器
     *
     */
    void connect_to_host();

};

#endif // DECODER_TCPSOCKET_H
