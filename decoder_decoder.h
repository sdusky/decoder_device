#ifndef DECODER_DECODER_H
#define DECODER_DECODER_H

#include <QObject>
#include <stdio.h>
#include <QBitmap>
extern "C"
{
#include "MFC_API/SsbSipMfcApi.h"
#include "MFC_API/mfc_interface.h"
#include "MFC_API/tiletolinear.h"
}
#include "decoder_package.h"


/**
 * @brief 硬件解码器类，封装三星MFC_API函数
 *        邢大天 2014-12-06
 *        邢大天 2015-02-05
 *
 */
class decoder_decoder : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit decoder_decoder(QObject *parent = 0);

    /**
     * @brief 解码器初始化
     * @other 步骤：1.SsbSipMfcDecInit设置sps pps
     *             2.SsbSipMfcDecGetOutBuf 检查初始化是否成功
     *             3.SsbSipMfcDecGetOutBuf 检查输出缓冲区
     * @see   SSBSIP_MFC_ERROR_CODE SsbSipMfcEncInit(void *openHandle, void *param);
     * @see   SSBSIP_MFC_ERROR_CODE SsbSipMfcEncGetOutBuf(void *openHandle, SSBSIP_MFC_ENC_OUTPUT_INFO *output_info);
     * @return int 返回码
     */
    int decoder_init();
signals:

    /**
     * @brief   信号，显示图片
     *
     * @param vid_buffer 解码图片内容指针
     * @param width      解码图片宽度
     * @param height     解码图片高度
     */
    void show_video(unsigned char *vid_buffer,int width,int height);

    /**
     * @brief 显示黑屏
     *
     */
    void show_black();
private:

    /**
     * @brief 色彩空间转换NV12tiled->rgb888
     *        nv12t yuv420一种存储方式 yyyy uvuv 64×32 块打包方式（tiled）
     *
     * @see   void decoder_yuv420p_to_rgb888(unsigned char *yuv420,unsigned char *rgb888,int width,int height);
     * @param out 解码器输出
     */
    void decoder_nv12t_to_rgb888(SSBSIP_MFC_DEC_OUTPUT_INFO out);

    /**
     * @brief 色彩空间转换yuv420p->rgb888
     *
     * @param yuv420  Yuv420p所在空间 yyyy u v
     * @param rgb888  rgb888所在空间 rgb888
     * @param width   图片宽度
     * @param height  图片高度
     */
    void decoder_yuv420p_to_rgb888(unsigned char *yuv420,unsigned char *rgb888,int width,int height);

    /**
     * @brief 打开解码器
     * @other 步骤：1.SsbSipMfcDecOpen();打开编码器
     *             2.SsbSipMfcDecGetInBuf();设置解码内存地址
     * @see void *SsbSipMfcEncOpen(void *value);
     * @see SSBSIP_MFC_ERROR_CODE SsbSipMfcEncGetInBuf(void *openHandle, SSBSIP_MFC_ENC_INPUT_INFO *input_info);
     *
     * @return int 返回码
     */
    int decoder_open();


private:
    void *decoder_handle;               /**< 解码器句柄 */
    SSBSIP_MFC_ERROR_CODE ret;          /**< 解码器返回值 */
    int max_frame_size;                 /**< 最大帧数据 */
    void* vir_inbuf;                    /**< 虚拟内存地址 */
    void* phy_inbuf;                    /**< 硬件内存地址 */
    int frame_count;                    /**< 帧数量 */
    unsigned char *fbuf;                /**< 开辟内存指针 */
public slots:

    /**
     * @brief 槽函数，解码帧数据
     * @other 步骤：1.SsbSipMfcDecExe() 解码视频流;
     *             2.SsbSipMfcDecGetOutBuf();获得解码输出
     *             3.decoder_nv12t_to_rgb888();色彩空间转换
     * @param buffer 需解码内存首地址
     * @param length 内容长度
     */
    void decoder_frame(void *buffer,int length);
};

#endif // DECODER_DECODER_H
