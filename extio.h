#ifndef EXTIO_H
#define EXTIO_H

#include <QObject>
#include <libftdi1/ftdi.h>

class ExtIO : public QObject
{
    Q_OBJECT
public:
    explicit ExtIO(QObject *parent = 0);
    ~ExtIO(void);

    int connectFtdi(void);
    uint8_t gateState(void);

signals:
    void logMessage(QString);
public slots:

private:
    bool readPins(void);

    struct ftdi_context *ftdi;
    uint8_t ftdi_pins;
};

#endif // EXTIO_H
