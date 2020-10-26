#include "modules335.h"

#include "qbuscontroller.h"
#include "camacdef.h"

ModuleS335::ModuleS335(QObject *parent)
{

}

void ModuleS335::stopCounters(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_DISABLE, stationNumber(), 0);
}

uint16_t ModuleS335::readCounter(uint8_t channel)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), channel);
    return qbusController()->dataRegister();
}

void ModuleS335::resetCounters(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_CLEAR_GROUP2_REGISTER, stationNumber(), 0);
}
