#include "modulecounter8.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleCounter8::ModuleCounter8(QObject *parent)
{

}

uint32_t ModuleCounter8::readCounter(uint8_t index)
{
    uint32_t return_value;

    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), index * 2);
    return_value = qbusController()->dataRegister();
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), (index * 2) + 1);
    return_value |= (uint32_t) qbusController()->dataRegister() << 16;

    return return_value;
}

uint16_t ModuleCounter8::readButtons(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP2_REGISTER, stationNumber(), 0);
    return qbusController()->dataRegister();
}

void ModuleCounter8::resetCounters(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_CLEAR_GROUP1_REGISTER, stationNumber(), 0);
}
