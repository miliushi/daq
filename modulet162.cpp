#include "modulet162.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleT162::ModuleT162(QObject *parent)
{
    (void)parent;
}

void ModuleT162::writeMemory(uint16_t address, uint16_t value)
{
    qbusController()->setDataRegister(value);
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_OVERWRITE_GROUP1_REGISTER, stationNumber(), address);
}

uint16_t ModuleT162::readMemory(uint16_t address)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), address);
    return qbusController()->dataRegister();
}

void ModuleT162::outputPulse(uint16_t channel)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_EXECUTE, stationNumber(), channel);
}
