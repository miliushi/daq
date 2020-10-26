#include "modulebrrpk32.h"
#include "qbuscontroller.h"
#include "camacdef.h"

ModuleBrrpk32::ModuleBrrpk32(QObject *parent)
{

}

void ModuleBrrpk32::reset(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_CLEAR_GROUP1_REGISTER, stationNumber(), 0);
}

void ModuleBrrpk32::setTimeWindow(TimeWindow value)
{
    qbusController()->setDataRegister(value);
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_OVERWRITE_GROUP1_REGISTER, stationNumber(), 0);
}

void ModuleBrrpk32::startBRRPK(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_EXECUTE, stationNumber(), 0);
}

uint16_t ModuleBrrpk32::readAndClearGroup1(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_READ_AND_CLEAR_GROUP1_REGISTER, stationNumber(), 0);
    return qbusController()->dataRegister();
}

bool ModuleBrrpk32::testStatus(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_TEST_STATUS, stationNumber(), 0);
    return qbusController()->statusQ();
}
