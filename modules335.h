#ifndef MODULES335_H
#define MODULES335_H

#include "qbusmodule.h"
#include <stdint.h>

class ModuleS335 : public QbusModule
{
    Q_OBJECT
public:
    ModuleS335(QObject *parent = 0);

    void stopCounters(void);
    uint16_t readCounter(uint8_t channel);
    void resetCounters(void);
};

#endif // MODULES335_H
