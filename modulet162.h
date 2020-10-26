#ifndef MODULET162_H
#define MODULET162_H

#include "qbusmodule.h"

class ModuleT162 : public QbusModule
{
    Q_OBJECT
public:
    ModuleT162(QObject *parent = 0);

    void writeMemory(uint16_t address, uint16_t value);
    uint16_t readMemory(uint16_t address);

    void outputPulse(uint16_t channel);
};

#endif // MODULET162_H
