#include "moduletl2.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleTl2::ModuleTl2(QObject *parent)
{

}

uint32_t ModuleTl2::readCounter(uint8_t channel)
{
    uint32_t return_value;

    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_AND_CLEAR_GROUP1_REGISTER, stationNumber(), channel);
    return_value = qbusController()->dataRegister();

    if(channel < 4)
    {
        qbusController()->executeCamacFunction(CAMAC_FUNCTION_NONSTANDARD_04, stationNumber(), channel);
        return_value |= (uint32_t) qbusController()->dataRegister() << 16;
    }

    return return_value;
}
