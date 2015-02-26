#ifndef DECODER_ALARM_H
#define DECODER_ALARM_H

#include <QObject>

/**
 * @brief 错误报警类
 * @todo
 */
class decoder_alarm : public QObject
{
    Q_OBJECT
public:
    explicit decoder_alarm(QObject *parent = 0);

signals:

public slots:

};

#endif // DECODER_ALARM_H
