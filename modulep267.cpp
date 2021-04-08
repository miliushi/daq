#include "modulep267.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleP267::ModuleP267(QObject *parent)
{
    (void) parent;
}

uint16_t ModuleP267::readDataRegister(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), MODULEP67_REGISTER_DATA);
    return qbusController()->dataRegister();
}

uint16_t ModuleP267::readMemory(uint16_t address)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), address);
    return qbusController()->dataRegister();
}
