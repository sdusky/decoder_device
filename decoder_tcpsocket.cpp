#include "decoder_tcpsocket.h"

decoder_tcpsocket::decoder_tcpsocket(QString hostip,int hostport)
{
    host_ip = hostip;
    host_port = hostport;

}

void decoder_tcpsocket::get_data_from_host()
{
    data_from_host = this->readAll();
    if(data_from_host == "what are you")
    {
        this->write("i am a decoder");
        return;
    }
    data_parse(data_from_host);
}

void decoder_tcpsocket::data_parse(QByteArray data)
{
    int length = data.left(4).toInt(NULL,10);  /**< 数据长度 */
    if(length <= 0)
        return;

    QByteArray code = data.mid(4,4);
    if(code.operator ==(GET_DEVICE_TIME))
    {
        send_info_to_host(GET_DEVICE_TIME,
                          QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
    else if(code.operator ==(GET_DEVICE_ADDRESS))
    {
        send_info_to_host(GET_DEVICE_ADDRESS,
                          device_setting_read.settings_addr);
    }
    else if(code.operator ==(GET_DEVICE_STATE))
    {
        send_info_to_host(GET_DEVICE_STATE,
                          QString::number(device_setting_read.settings_state).toLatin1());
    }
    else if(code.operator ==(GET_DEVICE_HEART))
    {
        send_info_to_host(GET_DEVICE_HEART,
                          "0000");
    }
    else if(code.operator ==(SET_DEVICE_TIME))
    {
        QByteArray current_time = data.mid(8,19);
        char set_time[50];
        sprintf(set_time,"time -s \" %s \"\n",current_time.data());
        system(set_time);
    }
    else if(code.operator ==(SET_DEVICE_STATE))
    {
        int state = data.mid(8,1).toInt();
        change_device_state(state);
    }
    else if(code.operator ==(SET_DEVICE_EXIT))
    {
        this->close();
        system("reboot\n");
    }
}

void decoder_tcpsocket::change_device_state(int state_num)
{
    device_setting_read.settings_state = state_num;
}


void decoder_tcpsocket::send_info_to_host(QString op_code, QString op_data)
{
    data_to_host.clear();
    data_to_host.append(op_code);
    data_to_host.append(op_data);

    QByteArray array_length;
    array_length = QString::number(data_to_host.size()).toLatin1();
    while(array_length.size() < 4)
        array_length.insert(0,'0');

    data_to_host.insert(0,array_length);
    this->write(data_to_host);
}

void decoder_tcpsocket::connect_to_host()
{
    this->connectToHost(host_ip,host_port);
    data_from_host.clear();
    while(!this->waitForConnected(30000))
    {
        this->connectToHost(host_ip,host_port);
    }
}
