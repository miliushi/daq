#include "modulele83.h"
#include "qbuscontroller.h"
#include "camacdef.h"
#include "qbusline.h"


modulele83::modulele83(QObject *parent) : QbusController(parent)
{
    setCrateAddress(0);
}

bool modulele83::setCrateAddress(int addr)
{
    if(addr < 0 || addr > 15)
    {
        return false;
    }
    else
    {
        crate_addr_base = LE83_BASE_ADDR + (addr * 0x10);
        return true;
    }
}

uint16_t modulele83::readAddrRegister(void)
{
    uint16_t addr;
    bool ok;

    ok = readRegister(LE83_REGISTER_ADDR, &addr);

    if(ok)
    {
        return addr;
    }
    else
    {
        return 0;
    }
}

uint16_t modulele83::readDataRegister(void)
{
    uint16_t data;
    bool ok;

    ok = readRegister(LE83_REGISTER_DATA, &data);

    if(ok)
    {
        return data;
    }
    else
    {
        return 0;
    }
}

uint16_t modulele83::statusRegister(void)
{
    uint16_t status;
    bool ok;

    ok = readRegister(LE83_REGISTER_STATUS, &status);

    if(ok)
    {
        return status;
    }
    else
    {
        return 0;
    }
}

uint16_t modulele83::statusRegime(void)
{
    uint16_t status;
    bool ok;

    ok = readRegister(LE83_REGIME_STATUS, &status);

    if(ok)
    {
        return status;
    }
    else
    {
        return 0;
    }
}

uint16_t modulele83::statusInformation(void)
{
    if(statusRegister() & 0x800)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t modulele83::statusErrorRegime(void)
{
    if(statusRegime() & 0x8000)
    {

        qDebug("Error: OSH, ");
        if(statusRegime() & 4000)
            qDebug("miss problem 4000");
        if(statusRegime() & 2000)
            qDebug("miss problem 2000");
        if(statusRegime() & 1000)
            qDebug("miss problem 1000 ");
        qDebug("\n");
        return 1;
    }
    else
    {
        return 0;
    }
}


int modulele83::getNcycle(void)
{
    int Nc;
    Nc = statusRegime() & 0x3FF;
    return Nc;
}

uint16_t modulele83::setOS(void)
{
    bool ok;

    ok = writeRegister(LE83_REGISTER_COMMAND, 0x8000);

    if(ok)  return  1;
    else    return  0;
}

uint16_t modulele83::setRegim(void)
{
    bool ok;

    ok = writeRegister(LE83_REGISTER_REGIME,0x180);

    if(ok)  return 1;
    else    return 0;
}

uint16_t modulele83::ResetOS(void)
{
    bool ok;

    ok = writeRegister(LE83_REGISTER_COMMAND,0x8000);

    if(ok)  return 1;
    else    return 0;
}

bool modulele83::writeRegister(uint8_t le83_register, uint16_t value)
{
    return qbusLine()->writeRegister(LE83_BASE_ADDR + le83_register, value);
}

bool modulele83::readRegister(uint8_t le83_register, uint16_t* value)
{
    return qbusLine()->readRegister(LE83_BASE_ADDR + le83_register, value);
}
