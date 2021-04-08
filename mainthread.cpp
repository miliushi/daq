#include "mainthread.h"

#include "spindaq.h"
#include "extiothread.h"

MainThread::MainThread()
{
    abort = false;
    daq_is_running = false;

    gate_begin_flag = false;
    gate_end_flag = false;

    ext_io_thread = new ExtIOThread();
    connect(ext_io_thread, SIGNAL(fatalError()), this, SLOT(extioFatalError()));

    spin_daq = new SpinDAQ;
    connect(ext_io_thread, SIGNAL(logMessage(QString)), spin_daq, SLOT(appendLogMessage(QString)));
    connect(ext_io_thread, SIGNAL(gateBegin()), this, SLOT(gateBegin()));
    connect(ext_io_thread, SIGNAL(gateEnd()), this, SLOT(gateEnd()));
    connect(ext_io_thread, SIGNAL(startDaq()), this, SLOT(startDaq()));
    connect(ext_io_thread, SIGNAL(stopDaq()), this, SLOT(stopDaq()));

    output = true;
}

MainThread::~MainThread(void)
{
    abort = true;
    wait();

    delete ext_io_thread;
    delete spin_daq;
}

void MainThread::run(void)
{
    bool ok;

    ext_io_thread->start(QThread::NormalPriority);
    //while(1)
    {
    }

    //ok = spin_daq->initialize();
    //if(ok)
    {
        spin_daq->startQbus();

        while(!abort)
        {
            if(daq_is_running)
            {
                ok = spin_daq->initialize();

                //test
                if(spin_daq->testCamac())
                {
                    spin_daq->appendLogMessage("brrpk1 ok");
                }
                else
                {
                    spin_daq->appendLogMessage("brrpk1 test LAM failed");
                }

                //

                spin_daq->appendLogMessage("daq start");

                // before start
                waitGateEnd();
                if(!abort)
                {
                    spin_daq->appendLogMessage("daq is running");
                    if(spin_daq->Run_Setup_Camac())
                    {
                        spin_daq->appendLogMessage("daq setup is finished");
                        gate_begin_flag = false;

                        while(daq_is_running && !abort)
                        {
                            waitGateBegin();
                            // bs
                            spin_daq->fill_BS_record();

                            spin_daq->Begin_Spill_Setup_Camac();

                            // Polling_Trigger_1
                            gate_end_flag = false;
                            while(!gate_end_flag && daq_is_running && !abort)
                            {
                                if(spin_daq->Trigger_1_detected())
                                {
                                    spin_daq->fill_Trig1_record();
                                    spin_daq->Unblock_Trigger_1();
                                }
                            }

                            if(spin_daq->End_Spill_Setup_Camac())
                            {
                                spin_daq->fill_ES_record();
                                // todo
                                bool send_data_ok = spin_daq->sendData();
                                if(!send_data_ok)
                                {
                                    spin_daq->appendLogMessage("sendData error");
                                }
                            }
                            else
                            {
                                abort = true;
                            }
                        }
                    }
                    else
                    {
                        abort = true;
                    }
                }
                spin_daq->deinit();
            }
            msleep(1);
        }
        spin_daq->stopQbus();
    }

    msleep(100);
    //spin_daq->deinit();

    emit finished();
}

void MainThread::gateBegin(void)
{
    gate_begin_flag = true;
    if(output){
        qDebug("gateBegin");
    }
}

void MainThread::gateEnd(void)
{
    gate_end_flag = true;
    if(output){
        qDebug("gateEnd");
    }
}

void MainThread::extioFatalError(void)
{
    abort = true;
}

void MainThread::startDaq(void)
{
    daq_is_running = true;
}

void MainThread::stopDaq(void)
{
    daq_is_running = false;
}

void MainThread::waitGateBegin(void)
{
    gate_begin_flag = false;
    while(!gate_begin_flag && daq_is_running && !abort)
    {
        msleep(1);
    }
}

void MainThread::waitGateEnd(void)
{
    if(output){
        qDebug("waiting for the gate end");
    }
    gate_end_flag = false;
    while(!gate_end_flag && daq_is_running && !abort)
    {
        msleep(1);
    }
    if(output){
        qDebug("gate end");
    }}
