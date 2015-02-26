#include "decoder_settings.h"

decoder_settings::decoder_settings(QObject *parent, QString hostip, int hostport)
    : QTcpSocket(parent)
{
    host_ip = hostip;
    host_port = hostport;
}

void decoder_settings::connect_to_host()
{
    this->connectToHost(host_ip,host_port);
    while(!this->waitForConnected(30000))
    {
         this->connectToHost(host_ip,host_port);
    }
    qDebug()<<"Connected";
}

void decoder_settings::get_data_from_host()
{
    data_from_host = this->readAll();
    data_parse(data_from_host);
}

void decoder_settings::data_parse(QByteArray data)
{
    int length = data.left(4).toInt(NULL,10); /**< 数据长度 4个字节 */
    if(length <= 0)
        return;

    QByteArray code = data.mid(4,4);          /**< 数据内容 */
    ///当配置地址、服务器ip、本地ip、本地类型、本地mac等信息时写入文件
    if(code.operator ==(SETTING_CODE_ADDRESS) ||
            code.operator ==(SETTING_CODE_HOSTIP) ||
            code.operator ==(SETTING_CODE_LOCALIP) ||
            code.operator ==(SETTING_CODE_TYPE) ||
            code.operator ==(SETTING_CODE_LOCALMAC))
    {
        write_settings_to_file(code,
                               data.mid(8,length - 8));
    }
    else if(code.operator ==(SETTING_CODE_FILENAME))
    {
        ///todo
    }
    else if(code.operator ==(SETTING_CODE_FILEEND))
    {
        ///todo
    }
    else if(code.operator ==(SETTING_CODE_RESET))
    {
        QByteArray data_to_host;
        data_to_host.append(SETTING_CODE_RESET);
        data_to_host.append("0000");
        this->write(data_to_host);
        system("reboot");
    }


}

void decoder_settings::write_settings_to_file(QByteArray op_code, QByteArray op_data)
{
    QSettings settings_file;
    if(op_code.operator ==(SETTING_CODE_ADDRESS))
    {
        settings_file.setValue("/basic_information/address",op_data);
    }
    else if(op_code.operator ==(SETTING_CODE_HOSTIP))
    {
        settings_file.setValue("/net_information/hostip",op_data);
    }
    else if(op_code.operator ==(SETTING_CODE_LOCALIP))
    {
        settings_file.setValue("/net_information/localip",op_data);
    }
    else if(op_code.operator ==(SETTING_CODE_TYPE))
    {
        settings_file.setValue("/basic_information/devce_type",op_data);
    }
    else if(op_code.operator ==(SETTING_CODE_LOCALMAC))
    {
        settings_file.setValue("/net_information/localmac",op_data);
    }

    QByteArray data_to_host;
    data_to_host.append(op_code);
    data_to_host.append("0000");
    this->write(data_to_host);
}
