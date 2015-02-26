#include "mainwindow.h"
#include <QApplication>
#include "decoder_package.h"
#include <QThread>
#include "decoder_decoder.h"
#include "decoder_information_parse.h"
#include "decoder_settings.h"
#include "decoder_tcpsocket.h"
#include "decoder_udpsocket.h"

sensor_data sensors_data[10];
settings_info device_setting_read;
settings_info device_setting_write;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    upload_settings(&device_setting_read);
    /*需要一段程序加载 TODO*/
    QThread                     tcp_thread;         /**< tcp线程 */
    QThread                     udp_thread;         /**< udp线程 */
    QThread                     settings_thread;    /**< 配置线程 */
    QThread                     decoder_thread;     /**< 解码线程 */
    qDebug()<<device_setting_read.settings_hostip;
    decoder_decoder decoder;
    decoder_settings set_tcp(NULL,device_setting_read.settings_hostip,5000);
    decoder_tcpsocket con_tcp(device_setting_read.settings_hostip,6000);
    decoder_udpsocket udp(7000);
    decoder_information_parse information;

    udp.moveToThread(&udp_thread);
    decoder.moveToThread(&decoder_thread);
    con_tcp.moveToThread(&tcp_thread);
    set_tcp.moveToThread(&settings_thread);

    MainWindow w;

    ///信号与槽连接
    QObject::connect(&decoder,SIGNAL(show_black()),
                     &w,SLOT(show_blank()));
    QObject::connect(&decoder,SIGNAL(show_video(unsigned char*,int,int)),
                     &w,SLOT(show_video(unsigned char*,int,int)));
    QObject::connect(&information,SIGNAL(send_video_to_decoder(void*,int)),
                     &decoder,SLOT(decoder_frame(void*,int)));
    QObject::connect(&information,SIGNAL(show_sensor_data(int)),
                     &w,SLOT(show_sensor(int)));
    QObject::connect(&information,SIGNAL(show_system_data(QString)),
                     &w,SLOT(show_system_data(QString)));
    QObject::connect(&udp,SIGNAL(send_data_to_information(QByteArray)),
                     &information,SLOT(get_data_from_udpsocket(QByteArray)),Qt::DirectConnection);
    QObject::connect(&udp,SIGNAL(send_ip_to_information(QString)),
                     &information,SLOT(get_remoteip_from_udpsocket(QString)),Qt::DirectConnection);
    QObject::connect(&con_tcp,SIGNAL(readyRead()),
                     &con_tcp,SLOT(get_data_from_host()));
    QObject::connect(&set_tcp,SIGNAL(readyRead()),
                     &set_tcp,SLOT(get_data_from_host()));
    QObject::connect(&udp,SIGNAL(readyRead()),
                     &udp,SLOT(get_data_from_encoder()));

    QObject::connect(&tcp_thread,SIGNAL(started()),&con_tcp,SLOT(connect_to_host()));

    QObject::connect(&settings_thread,SIGNAL(started()),&set_tcp,SLOT(connect_to_host()));

    ///ui显示
    w.show();

    ///线程开启
    decoder_thread.start();
    tcp_thread.start();
    udp_thread.start();
    settings_thread.start();

    return a.exec();
}

void upload_settings(settings_info* info)
{
    QSettings ini_file("/settings/device_settings.ini",QSettings::IniFormat);
    info->settings_time    =
            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toLatin1();
    info->settings_addr    =
            ini_file.value("/basic_information/address").toByteArray();
    info->settings_hostip  =
            ini_file.value("/net_information/hostip").toByteArray();
    info->settings_localmac  =
            ini_file.value("/net_information/localmac").toByteArray();
    info->settings_localip =
            ini_file.value("/net_information/localip").toByteArray();
}


