#ifndef MODULEP267_H
#define MODULEP267_H

#include "qbusmodule.h"

#define MODULEP67_REGISTER_DATA        0x00

class ModuleP267 : public QbusModule
{
    Q_OBJECT
public:
    ModuleP267(QObject *parent = 0);

    uint16_t readDataRegister(void);
    uint16_t readMemory(uint16_t address);


};

#endif // MODULEP267_H
