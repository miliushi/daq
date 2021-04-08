#ifndef MODULELE83_H
#define MODULELE83_H

#include "qbuscontroller.h"

#define LE83_BASE_ADDR				0177040

#define LE83_REGISTER_STATUS        00
#define LE83_REGISTER_COMMAND       02
#define LE83_REGISTER_REGIME        06
#define LE83_REGIME_STATUS          012
#define LE83_REGISTER_ADDR          014
#define LE83_REGISTER_DATA          016


class QbusLine;

class modulele83 : public QbusController
{
    Q_OBJECT
public:
    explicit modulele83(QObject *parent = 0);

    bool setCrateAddress(int addr);

    uint8_t regime(void);

    uint16_t statusRegister(void);
    uint16_t statusRegime(void);
    uint16_t statusInformation(void);   // Record data status
    uint16_t statusErrorRegime(void);
    uint16_t readAddrRegister(void);
    uint16_t readDataRegister(void);

    uint16_t setOS(void);           // General Reset
    uint16_t setRegim(void);        // set regim le83
    uint16_t ResetOS(void);

    int getNcycle(void);            // Get number data cycle

private:
    bool writeRegister(uint8_t le83_register, uint16_t value);
    bool readRegister(uint8_t le83_register, uint16_t* value);

    uint16_t crate_addr_base;

};


#endif // MODULELE83_H
