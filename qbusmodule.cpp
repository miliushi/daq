#include "qbusmodule.h"
#include "qbuscontroller.h"
#include "camacdef.h"

QbusModule::QbusModule(QObject *parent) : QObject(parent)
{
    qbus_controller = NULL;
}

void QbusModule::setQbusController(QbusController* controller)
{
    qbus_controller = controller;
}

QbusController* QbusModule::qbusController(void)
{
    return qbus_controller;
}

void QbusModule::setStationNumber(int number)
{
    station_number = number;
}

int QbusModule::stationNumber(void)
{
    return station_number;
}

void QbusModule::setModuleName(QString name)
{
    module_name = name;
}

QString QbusModule::ModuleName(void)
{
    return module_name;
}

bool QbusModule::testLAM(void)
{
    qbusController()->executeCamacFunction(CAMAC_FUNCTION_TEST_LOOK_AT_ME, stationNumber(), 0);
    return qbusController()->statusQ();
}
