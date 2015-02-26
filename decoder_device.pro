#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T09:36:51
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = decoder_device
TEMPLATE = app

LIBS += /usr/lib/tile_to_linear.so

SOURCES += main.cpp\
        mainwindow.cpp \
    decoder_tcpsocket.cpp \
    decoder_udpsocket.cpp \
    decoder_information_parse.cpp \
    decoder_settings.cpp \
    decoder_alarm.cpp \
    decoder_decoder.cpp \
    MFC_API/SsbSipMfcDecAPI.c

HEADERS  += mainwindow.h \
    decoder_package.h \
    decoder_tcpsocket.h \
    decoder_udpsocket.h \
    decoder_information_parse.h \
    decoder_settings.h \
    decoder_alarm.h \
    decoder_decoder.h \
    MFC_API/SsbSipMfcApi.h \
    MFC_API/mfc_interface.h \
    MFC_API/tiletolinear.h

FORMS    += mainwindow.ui


