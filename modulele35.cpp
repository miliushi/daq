#include "modulele35.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleLe35::ModuleLe35(QObject *parent)
{
    (void) parent;
}

uint16_t ModuleLe35::readDataRegister(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_GROUP1_REGISTER, stationNumber(), MODULELE35_REGISTER_DATA);
    return qbusController()->dataRegister();
}


