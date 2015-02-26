#include "decoder_information_parse.h"

decoder_information_parse::decoder_information_parse(QObject *parent) :
    QObject(parent)
{

}

void decoder_information_parse::get_data_from_udpsocket(QByteArray original_data)
{

    int pkt_length = original_data.mid(0,6).toInt();      /**< 包长度 6字节 */
    const int sen_num = original_data.mid(6 , 1).toInt(); /**< 传感器数量 1字节 */

    if(pkt_length == original_data.size())
    {
        int vid_length = original_data.mid(7,6).toInt(); /**< 视频信息长度 6字节 */

        void *vid_buffer = original_data.mid(13,vid_length).data(); /**< 视频信息内容 */
        emit send_video_to_decoder(vid_buffer,vid_length);

        int sen_length = original_data.mid(13 + vid_length,4).toInt(); /**< 传感器信息长度 */

        if(sen_num > 0)
        {
            QByteArray sensor_buffer = original_data.mid(17 + vid_length,sen_length);

            int i;
            int sensor_name_length = 0; /**< 传感器名称长度 4字节 */
            int sensor_info_length = 0; /**< 传感器信息长度 4字节 */
            for(i = 0; i < sen_num; i++)
            {
                sensor_name_length = sensor_buffer.mid(0,4).toInt();
                sensor_buffer.remove(0,4);

                sensors_data[i].sensor_name = sensor_buffer.mid(0,sensor_name_length);
                sensor_buffer.remove(0,sensor_name_length);

                sensor_info_length = sensor_buffer.mid(0,4).toInt();
                sensor_buffer.remove(0,4);

                sensors_data[i].sensor_info = sensor_buffer.mid(0,sensor_info_length);
                sensor_buffer.remove(0,sensor_info_length);
            }

            emit show_sensor_data(sen_num);
        }
    }
}

void decoder_information_parse::get_remoteip_from_udpsocket(QString remoteip)
{
    emit show_system_data(remoteip);
}
