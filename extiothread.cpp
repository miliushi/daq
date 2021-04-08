#include "extiothread.h"
#include "extio.h"
#include <QFile>
#include <QTextStream>

ExtIOThread::ExtIOThread()
{
    abort = false;
    gate_state = 0;

    ext_io = new ExtIO(this);
    connect(ext_io, SIGNAL(logMessage(QString)), this, SIGNAL(logMessage(QString)));
}

ExtIOThread::~ExtIOThread(void)
{
    abort = true;
    wait();

    delete ext_io;
}

void ExtIOThread::run(void)
{
    uint8_t new_gate_state;
    int status;

    status = ext_io->connectFtdi();
    if(status == EXIT_FAILURE)
    {
        emit fatalError();
        return;
    }

    while(!abort)
    {
        new_gate_state = ext_io->gateState();
        if(new_gate_state != gate_state)
        {
            gate_state = new_gate_state;
            if(gate_state == 1)
            {
                emit gateBegin();
            }
            else
            {
                emit gateEnd();
            }
        }
        readControl();
        msleep(10);
    }
}

void ExtIOThread::readControl(void)
{
    static bool last_daq_enable = false;
    QFile daq_enable_file("/home/daq/project/qt/build-daq-Desktop_Qt_5_7_0_GCC_64bit-Release/daq_enable.txt");
    daq_enable_file.open(QFile::ReadOnly);
    QTextStream daq_enable_stream(&daq_enable_file);
    bool daq_enable = daq_enable_stream.readAll().toInt();
    if(daq_enable != last_daq_enable)
    {
        last_daq_enable = daq_enable;
        if(daq_enable)
        {
            logMessage("start DAQ");
            emit startDaq();
        }
        else
        {
            emit stopDaq();
            logMessage("stop DAQ");
        }
    }
}

