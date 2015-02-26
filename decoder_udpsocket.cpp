#include "decoder_udpsocket.h"

decoder_udpsocket::decoder_udpsocket(int port)
{
    this->bind(port, QUdpSocket::ShareAddress);

    data_from_encoder.clear();
}

void decoder_udpsocket::get_data_from_encoder()
{
    while(this->hasPendingDatagrams())
    {
       ///pendingDatagramSize为返回第一个在等待读取报文的size，resize函数是把datagram的size归一化到参数size的大小一样
       data_from_encoder.resize(this->pendingDatagramSize());
       this->readDatagram(data_from_encoder.data(),data_from_encoder.size());
       emit send_data_to_information(data_from_encoder);
       emit send_ip_to_information(this->peerAddress().toString());
    }
}
