#include "decoder_decoder.h"
#include <QFile>
decoder_decoder::decoder_decoder(QObject *parent) :
    QObject(parent)
{
    decoder_handle  = NULL;
    vir_inbuf       = NULL;
    phy_inbuf       = NULL;
    ret             = MFC_RET_OK;
    max_frame_size  = MAX_DECODER_INPUT_BUFFER_SIZE;
    fbuf = (unsigned char *)malloc(sizeof(max_frame_size));
    decoder_open();
    decoder_init();
}

int decoder_decoder::decoder_open()
{
    qDebug()<<"Open!";
    SSBIP_MFC_BUFFER_TYPE buf_type = CACHE;  /**< 解码器打开方式 */
    decoder_handle = SsbSipMfcDecOpen(&buf_type);
    if(decoder_handle == NULL)
    {
        printf("failed to open mfc device!\n");
        return -1;
    }
    printf(" SsbSipMfcDecOpen OK \n");
    vir_inbuf = SsbSipMfcDecGetInBuf(decoder_handle, &phy_inbuf, max_frame_size);
    printf("== SsbSipMfcDecGetInBuf OK \n");
    printf("vir_inbuf=%08x, phy_inbuf=%08x \n", vir_inbuf, phy_inbuf);
    return 1;
}
int decoder_decoder::decoder_init()
{
    /* 得到 输入缓冲区信息 */
    QByteArray spspps; /**< sps pps 固定信息 */

    spspps.append(QByteArray::fromHex("00 00 00 01 67 64 00 28 ac d3 02 80 f6 40"));
    spspps.append(QByteArray::fromHex("00 00 00 01 68 ea 40 2f cb"));

    memcpy(vir_inbuf,spspps.data(),spspps.size());
     /* 初始化 */
    ret = SsbSipMfcDecInit(decoder_handle, H264_DEC, spspps.size());
    if(ret != MFC_RET_OK)
    {
        printf("failed to init mfc!\n");
        return -1;
    }
    else
    {
        /// 检查是否初始化成功
        SSBSIP_MFC_DEC_OUTPUT_INFO out_info; /**< 缓冲区输出信息 */
        SSBSIP_MFC_DEC_OUTBUF_STATUS status = SsbSipMfcDecGetOutBuf(decoder_handle, &out_info);
        if(out_info.img_width <= 0 || out_info.img_height <= 0)
        {

            printf("bad image size, should init again!\n");
            return -1;
        }
        qDebug()<<"image_width"<<out_info.img_width
                  <<"image_height"<<out_info.img_height;
    }
    ///检查输出缓冲区
    printf("== SsbSipMfcDecInit OK \n");

    SSBSIP_MFC_DEC_OUTPUT_INFO out;/**< 缓冲区输出信息 */
    SSBSIP_MFC_DEC_OUTBUF_STATUS status; /**< 输出状态 */
    status = SsbSipMfcDecGetOutBuf(decoder_handle, &out);
    printf("output: status = %d , image(%dx%d), buf(%dx%d), crop(%d,%d,%d,%d) \n",
          status,
          out.img_width, out.img_height,
          out.buf_width, out.buf_height,
          out.crop_left_offset, out.crop_top_offset,
          out.crop_right_offset, out.crop_bottom_offset);

    printf("-------------------------------\n");
    return 1;
}

void decoder_decoder::decoder_frame(void *buffer, int length)
{
    memcpy(vir_inbuf,buffer,length);
    ret = SsbSipMfcDecExe(decoder_handle, length);
    if(ret != MFC_RET_OK)
    {
//        printf("failed decoding (%d) \n", ret);
        return;
    }
    SSBSIP_MFC_DEC_OUTPUT_INFO out;
    SSBSIP_MFC_DEC_OUTBUF_STATUS status;
    status = SsbSipMfcDecGetOutBuf(decoder_handle, &out);
//    printf("<<< output: status = %d , image(%dx%d), buf(%dx%d), crop(%d,%d,%d,%d) \n",
//        status,
//        out.img_width, out.img_height,
//        out.buf_width, out.buf_height,
//        out.crop_left_offset, out.crop_top_offset,
//        out.crop_right_offset, out.crop_bottom_offset);

    if(status == MFC_GETOUTBUF_DISPLAY_DECODING
        || status == MFC_GETOUTBUF_DISPLAY_ONLY)
    {
        decoder_nv12t_to_rgb888(out);
    }
}

void decoder_decoder::decoder_nv12t_to_rgb888(SSBSIP_MFC_DEC_OUTPUT_INFO out)
{
    const int width = out.img_width;
    const int height = out.img_height;
    unsigned char rgb888[width * height * 3];
    unsigned char yuv420p[width * height * 2];
    tile_to_linear_64x32_4x2_neon(yuv420p,
                    (unsigned char *) out.YVirAddr, out.img_width,
                    out.img_height);

    tile_to_linear_64x32_4x2_uv_neon(
                    yuv420p + out.img_width * out.img_height,
                    (unsigned char *) out.CVirAddr, out.img_width,
                    out.img_height >> 1);

    decoder_yuv420p_to_rgb888(yuv420p,rgb888,width,height);

    emit show_video(rgb888,width,height);
}

void decoder_decoder::decoder_yuv420p_to_rgb888(unsigned char *yuv420,
                                                unsigned char *rgb888,
                                                int width,
                                                int height)
{
    int R,G,B,Y,U,V;
    unsigned char *py = yuv420;
    unsigned char *pu = yuv420 + width * height;
    unsigned char *pv = pu + (width * height) / 4;
    int x,y;
    int nWidth = width>>1; ///色度信号宽度
    for (y = 0;y < height; y++)
    {
        for (x=0; x < width; x++)
        {
            Y = *(py + y * width + x);
            U = *(pu + ((y >> 1) * nWidth) + (x >> 1));
            V = *(pv + ((y >> 1) * nWidth) + (x >> 1));
            R = Y + ((1437289 * (V - 128)) >> 20);
            G = Y - (((731907  * (V - 128)) - (354034 * (U - 128))) >> 20);
            B = Y + ((1816601 * (U - 128)) >> 20);

            ///防止越界
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;

            *(rgb888 + (y * width + x)* 3) = R;
            *(rgb888 + (y * width + x) * 3 + 1) = G;
            *(rgb888 + (y * width + x) * 3 + 2) = B;
        }
    }
}

