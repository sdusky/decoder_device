#ifndef DECODER_PACKAGE_H
#define DECODER_PACKAGE_H
#include <QByteArray>
#include <QDateTime>
#include <QSettings>
#include <QObject>
#include <QDebug>

/**
 * @brief 传感器结构体
 *
 */
typedef struct _sensor_data sensor_data;
struct _sensor_data{
    QByteArray sensor_name;                 /**< 传感器名称 */
    QByteArray sensor_info;                 /**< 传感器信息 */
};

/**
 * @brief 配置信息结构体
 *
 */
typedef struct _settings_info settings_info;
struct _settings_info{
    QByteArray settings_time;               /**< 系统时间 */
    QByteArray settings_addr;               /**< 系统安装地点 */
    QByteArray settings_localip;            /**< 系统ip */
    QByteArray settings_localmac;
    QByteArray settings_hostip;             /**< 服务器ip */
    int        settings_state;              /**< 系统状态 */
};
extern sensor_data sensors_data[10];        /**< 传感器数组 */
extern settings_info device_setting_read;   /**< 读取数据 */

#define GET_DEVICE_HEART           "0000"
#define GET_DEVICE_TIME            "0001"
#define GET_DEVICE_ADDRESS         "0002"
#define GET_DEVICE_STATE           "0003"

#define SET_DEVICE_TIME            "0011"
#define SET_DEVICE_STATE           "0013"
#define SET_DEVICE_EXIT            "0014"

#define SETTING_CODE_TYPE           "1001"
#define SETTING_CODE_ADDRESS        "1002"
#define SETTING_CODE_HOSTIP         "1003"
#define SETTING_CODE_LOCALIP        "1004"
#define SETTING_CODE_LOCALMAC       "1005"
#define SETTING_CODE_FILENAME       "1006"
#define SETTING_CODE_FILEEND        "1007"
#define SETTING_CODE_RESET          "1008"

#define ERROR_TCP_CONNECT           1
#define ERROR_SET_CONNECT           2

/**
 * @brief 提取当前信息
 *
 * @param info
 */
void upload_settings(settings_info* info);

#endif // DECODER_PACKAGE_H
