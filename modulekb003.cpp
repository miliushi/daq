#include "modulekb003.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleKb003::ModuleKb003(QObject *parent)
{
    (void) parent;
}

void ModuleKb003::setOutput(uint16_t mask)
{
    qbusController()->setDataRegister(mask);
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_OVERWRITE_GROUP1_REGISTER, stationNumber(), 0);
}
