#ifndef MODULECOUNTER8_H
#define MODULECOUNTER8_H

#include "qbusmodule.h"
#include <stdint.h>

class ModuleCounter8 : public QbusModule
{
    Q_OBJECT
public:
    ModuleCounter8(QObject *parent = 0);

    uint32_t readCounter(uint8_t index);
    uint16_t readButtons(void);
    void resetCounters(void);
};

#endif // MODULECOUNTER8_H
