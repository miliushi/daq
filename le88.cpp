#include "le88.h"
#include "le88def.h"
#include "qbusline.h"
#include "camacdef.h"



Le88::Le88(QObject *parent) : QbusController(parent)
{
    setCrateAddress(0);
}


bool Le88::setCrateAddress(int addr)
{
    if(addr < 0 || addr > 15)
    {
        return false;
    }
    else
    {
        crate_addr_base = LE88_BASE_ADDR_0 + (addr * 0x10);
        return true;
    }
}

int Le88::crateAddress(void)
{
    return (crate_addr_base - LE88_BASE_ADDR_0) / 0x10;
}

bool Le88::setDataRegister(uint16_t data)
{
    return writeRegister(LE88_REGISTER_DATA, data);
}

uint16_t Le88::dataRegister(void)
{
    uint16_t data;
    bool ok;

    ok = readRegister(LE88_REGISTER_DATA, &data);

    if(ok)
    {
        return data;
    }
    else
    {
        // read error
        return 0;
    }
}

uint16_t Le88::statusRegister(void)
{
    uint16_t status;
    bool ok;

    ok = readRegister(LE88_REGISTER_STATUS, &status);

    if(ok)
    {
        return status;
    }
    else
    {
        // read error
        return 0;
    }
}

uint16_t Le88::lamLRegister(void)
{
    uint16_t lam;
    bool ok;

    ok = readRegister(LE88_REGISTER_LAM_LOW, &lam);

    if(ok)
    {
        return lam;
    }
    else
    {
        // read error
        return 0;
    }
}

uint16_t Le88::lamHRegister(void)
{
    uint16_t lam;
    bool ok;

    ok = readRegister(LE88_REGISTER_LAM_HIGH, &lam);

    if(ok)
    {
        return lam;
    }
    else
    {
        // read error
        return 0;
    }
}

uint8_t Le88::statusQ(void)
{
    if(statusRegister() & LE88_STATUS_Q_BIT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t Le88::statusX(void)
{
    if(statusRegister() & LE88_STATUS_X_BIT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t Le88::readClearIn1(void)
{
    uint16_t status = statusRegister();

    if(status & LE88_STATUS_IN1_BIT)
    {
        writeRegister(LE88_REGISTER_STATUS, status & (LE88_STATUS_IN1_BIT ^ 0xffff));
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t Le88::readClearIn2(void)
{
    uint16_t status = statusRegister();

    if(status & LE88_STATUS_IN2_BIT)
    {
        writeRegister(LE88_REGISTER_STATUS, status & (LE88_STATUS_IN2_BIT ^ 0xffff));
        return 1;
    }
    else
    {
        return 0;
    }
}

void Le88::setOut3(uint8_t value)
{
    uint16_t status = statusRegister();
    uint16_t status_to_write;

    if(value)
    {
        status_to_write = status | LE88_STATUS_SY3_BIT;
    }
    else
    {
        status_to_write = status & (0xffff ^ LE88_STATUS_SY3_BIT);
    }

    writeRegister(LE88_REGISTER_STATUS, status_to_write);
}

void Le88::setOut4(uint8_t value)
{
    uint16_t status = statusRegister();
    uint16_t status_to_write;

    if(value)
    {
        status_to_write = status | LE88_STATUS_SY4_BIT;
    }
    else
    {
        status_to_write = status & (0xffff ^ LE88_STATUS_SY4_BIT);
    }

    writeRegister(LE88_REGISTER_STATUS, status_to_write);
}

bool Le88::executeCamacFunction(uint16_t function, uint16_t module, uint16_t address)
{
    uint16_t naf = function | ((module & 0x001f) << 4) | (address & 0x00ff);
    return writeRegister(LE88_REGISTER_COMMAND, naf);
}

bool Le88::generateZ(void)
{
    return executeCamacFunction(CAMAC_FUNCTION_ENABLE, 28, 8);
}

bool Le88::generateC(void)
{
    return executeCamacFunction(CAMAC_FUNCTION_ENABLE, 28, 9);
}

bool Le88::lowerCrateInhibit(void)
{
    return executeCamacFunction(CAMAC_FUNCTION_DISABLE, 30, 9);
}

bool Le88::raiseCrateInhibit(void)
{
    return executeCamacFunction(CAMAC_FUNCTION_ENABLE, 30, 9);
}

bool Le88::writeRegister(uint16_t le88_register, uint16_t value)
{
    return qbusLine()->writeRegister(crate_addr_base + le88_register, value);
}

bool Le88::readRegister(uint16_t le88_register, uint16_t* value)
{
    return qbusLine()->readRegister(crate_addr_base + le88_register, value);
}


