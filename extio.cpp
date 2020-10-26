#include "extio.h"

#define EXTIO_CBUS_GATE_MASK        0x08

ExtIO::ExtIO(QObject *parent) : QObject(parent)
{
    struct ftdi_version_info version;
    if ((ftdi = ftdi_new()) == 0)
    {

        fprintf(stderr, "ftdi_new failed\n");
        //return;
    }
    version = ftdi_get_library_version();
    qDebug((QString("libftdi v") + QString(version.version_str)).toLocal8Bit().constData());
}

ExtIO::~ExtIO(void)
{
    ftdi_usb_close(ftdi);
    ftdi_free(ftdi);
}

int ExtIO::connectFtdi(void)
{
    int ret;

    if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
    {
        emit logMessage("unable to open ftdi device: " + QString::number(ret) + " (" + QString(ftdi_get_error_string(ftdi)) + ")");
        return EXIT_FAILURE;
    }
    else
        emit logMessage("FTDI OK");

    if (ftdi->type == TYPE_R)
    {
        unsigned int chipid;
        printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
        printf("FTDI chipid: %X\n", chipid);
    }

    unsigned char bitmask = 0x00;   // all inputs
    ret = ftdi_set_bitmode(ftdi, bitmask, BITMODE_CBUS);
    if(ret < 0)
    {
        fprintf(stderr, "ftdi_set_bitmode failed for 0x%x, error %d (%s)\n", bitmask, ret, ftdi_get_error_string(ftdi));
        ftdi_usb_close(ftdi);
    }

    return EXIT_SUCCESS;
}

uint8_t ExtIO::gateState(void)
{
    readPins();
    if(ftdi_pins & EXTIO_CBUS_GATE_MASK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool ExtIO::readPins(void)
{
    int ret;
    unsigned char pins;

    ret = ftdi_read_pins(ftdi, &pins);
    if (ret < 0)
    {
        fprintf(stderr, "ftdi_read_pins failed, error %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_usb_close(ftdi);
        return false;
    }
    ftdi_pins = pins & 0x0f;
    return true;
}
