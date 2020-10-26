#ifndef LE88_H
#define LE88_H

#include "qbuscontroller.h"

class QbusLine;

class Le88 : public QbusController
{
    Q_OBJECT
public:
    explicit Le88(QObject *parent = 0);

    bool setCrateAddress(int addr);         // addr = 0..15
    int crateAddress(void);

    bool setDataRegister(uint16_t data);
    uint16_t dataRegister(void);
    uint16_t statusRegister(void);
    uint16_t lamLRegister(void);
    uint16_t lamHRegister(void);
    uint8_t statusQ(void);
    uint8_t statusX(void);
    uint8_t readClearIn1(void);
    uint8_t readClearIn2(void);
    void setOut3(uint8_t value);
    void setOut4(uint8_t value);

    bool executeCamacFunction(uint16_t function, uint16_t module, uint16_t address);

    bool generateZ(void);
    bool generateC(void);
    bool lowerCrateInhibit(void);
    bool raiseCrateInhibit(void);
signals:

public slots:

private:
    bool writeRegister(uint16_t le88_register, uint16_t value);
    bool readRegister(uint16_t le88_register, uint16_t* value);

    uint16_t crate_addr_base;
};

#endif // LE88_H
