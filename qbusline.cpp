#include "qbusline.h"
#include "qbus.h"
#include "remoteqbus.h"

QbusLine::QbusLine(QObject *parent) : QObject(parent)
{
    static char local_qbus_name[] = "/dev/qbus0";

    local_qbus = new QBUS(local_qbus_name);
    //remote_qbus = new RemoteQbus(this);

    connection_mode = CONNECTION_MODE_REMOTE;
}

QbusLine::~QbusLine(void)
{
    //delete remote_qbus;
}

bool QbusLine::start(void)
{
    if(connection_mode == CONNECTION_MODE_LOCAL)
    {
        return true;
    }
    else if (connection_mode == CONNECTION_MODE_REMOTE)
    {
        return remote_qbus->start();
    }
    return false;
}

bool QbusLine::stop(void)
{
    if(connection_mode == CONNECTION_MODE_LOCAL)
    {
        return true;
    }
    else if (connection_mode == CONNECTION_MODE_REMOTE)
    {
        return remote_qbus->stop();
    }
    return false;
}

void QbusLine::setDevice(QString device)
{
    if(connection_mode == CONNECTION_MODE_LOCAL)
    {

    }
    else if (connection_mode == CONNECTION_MODE_REMOTE)
    {
        remote_qbus->setDevice(device);
    }
}

bool QbusLine::writeRegister(uint16_t addr, uint16_t value)
{
    if(connection_mode == CONNECTION_MODE_LOCAL)
    {
        local_qbus->write(addr, value);
        return true;
    }
    else if (connection_mode == CONNECTION_MODE_REMOTE)
    {
       return  remote_qbus->writeRegister(addr, value);
    }
    return false;
}

bool QbusLine::readRegister(uint16_t addr, uint16_t* value)
{
    if(connection_mode == CONNECTION_MODE_LOCAL)
    {
        *value = local_qbus->read(addr);
        return true;
    }
    else if (connection_mode == CONNECTION_MODE_REMOTE)
    {
        return remote_qbus->readRegister(addr, value);
    }
    return false;
}

QbusLine::ConnectionMode QbusLine::connectionMode(void)
{
    return connection_mode;
}

void QbusLine::setConnectionMode(ConnectionMode mode)
{
    connection_mode = mode;
}
