#ifndef MODULEKB003_H
#define MODULEKB003_H

#include "qbusmodule.h"

class ModuleKb003 : public QbusModule
{
    Q_OBJECT
public:
    ModuleKb003(QObject *parent = 0);

    void setOutput(uint16_t mask);
private:

};

#endif // MODULEKB003_H
