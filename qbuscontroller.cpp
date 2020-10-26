#include "qbuscontroller.h"
#include "qbusline.h"

QbusController::QbusController(QObject *parent) : QObject(parent)
{
    qbus_line = NULL;
}

void QbusController::setQbusLine(QbusLine* qbus)
{
    qbus_line = qbus;
}

QbusLine* QbusController::qbusLine(void)
{
    return qbus_line;
}
