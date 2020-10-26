#ifndef QBUSCONTROLLER_H
#define QBUSCONTROLLER_H

#include <QObject>
class QbusLine;

class QbusController : public QObject
{
    Q_OBJECT
public:
    explicit QbusController(QObject *parent = 0);

    void setQbusLine(QbusLine* qbus);
    QbusLine* qbusLine(void);

    virtual bool setCrateAddress(int addr){(void) addr; return false;}
    virtual int crateAddress(void){return false;}
    virtual bool setDataRegister(uint16_t data){(void) data; return false;}
    virtual uint16_t dataRegister(void){return false;}
    virtual bool executeCamacFunction(uint16_t function, uint16_t module, uint16_t address){(void) function; (void) module; (void) address; return false;}

    virtual uint8_t statusQ(void){return false;}

signals:
public slots:
private:
    QbusLine* qbus_line;
};

#endif // QBUSCONTROLLER_H
