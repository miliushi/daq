#ifndef MODULELE35_H
#define MODULELE35_H

#include "qbusmodule.h"

#define MODULELE35_REGISTER_DATA        0x00

class ModuleLe35 : public QbusModule
{
    Q_OBJECT
public:
    ModuleLe35(QObject *parent = 0);

    uint16_t readDataRegister(void);
};

#endif // MODULELE35_H
