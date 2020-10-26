#ifndef MODULETL2_H
#define MODULETL2_H

#include "qbusmodule.h"
#include <stdint.h>

class ModuleTl2 : public QbusModule
{
    Q_OBJECT
public:
    ModuleTl2(QObject *parent = 0);

    uint32_t readCounter(uint8_t channel);
};

#endif // MODULETL2_H
