#ifndef QBUSLINE_H
#define QBUSLINE_H

#include <QObject>

class QBUS;
class RemoteQbus;

class QbusLine : public QObject
{
    Q_OBJECT
public:
    explicit QbusLine(QObject *parent = 0);
    ~QbusLine(void);


    bool start(void);
    bool stop(void);

    void setDevice(QString device);
    bool writeRegister(uint16_t addr, uint16_t value);
    bool readRegister(uint16_t addr, uint16_t* value);

    typedef enum
    {
        CONNECTION_MODE_LOCAL,
        CONNECTION_MODE_REMOTE
    }ConnectionMode;

    ConnectionMode connectionMode(void);
    void setConnectionMode(ConnectionMode mode);
signals:

public slots:
private:
    ConnectionMode connection_mode;

    QBUS* local_qbus;
    RemoteQbus* remote_qbus;
};

#endif // QBUSLINE_H
