#ifndef DECODER_SETTINGS_H
#define DECODER_SETTINGS_H

#include <QTcpSocket>
#include <QFile>
#include "decoder_package.h"

/**
 * @brief 设备配置类，继承自QTcpSocket类
 *
 *        邢大天 2014-12-06
 *
 */
class decoder_settings : public QTcpSocket
{
    Q_OBJECT
public:

    /**
     * @brief 构造函数获取服务器ip和服务器端口
     *
     * @param parent
     * @param hostip 服务器ip
     * @param hostport 服务器端口
     */
    explicit decoder_settings(QObject *parent,QString hostip,int hostport);
signals:

    /**
     * @brief 信号，传输错误信号
     *
     * @param error_no 错误号
     */
    void connect_error(int error_no);
public slots:

    /**
     * @brief 槽函数,接收服务器数据
     *
     */
    void get_data_from_host();

    /**
     * @brief 槽函数，连接到服务器
     *
     */
    void connect_to_host();
private:

    /**
     * @brief 解析函数 解析包
     *              信息格式为
     *              4字节长度 + 4字节操作码 + 操作数
     *              操作码：
     *              SETTING_CODE_ADDRESS       设置安置地点
     *              SETTING_CODE_HOSTIP        设置服务器IP
     *              SETTING_CODE_LOCALIP       设置本机IP
     *              SETTING_CODE_TYPE          设置设备类型(encoder or decoder)
     *              SETTING_CODE_FILENAME      发送文件名
     *              SETTING_CODE_FILEEND       发送文件结束
     *              SETTING_CODE_LOCALMAC      设置本机mac
     * @param data  要解析的数据
     */
    void data_parse(QByteArray data);

    /**
     * @brief 将数据写入文件
     *
     * @param op_code 操作码
     * @param op_data 操作数
     */
    void write_settings_to_file(QByteArray op_code,QByteArray op_data);

    QByteArray data_from_host;      /**< 接收服务器数据 */
    QString host_ip;                /**< 服务器ip */
    int host_port;                  /**< 服务器端口 */

};

#endif // DECODER_SETTINGS_H
