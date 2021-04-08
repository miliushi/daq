#include "spindaq.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QXmlStreamReader>
#include "dataclient.h"

#include "qbusline.h"
#include "le88.h"
#include "modulet162.h"
#include "modulebrrpk32.h"
#include "modulekb003.h"
#include "modulecounter8.h"
#include "modules335.h"
#include "moduletl2.h"
#include "modulep267.h"
#include "modulele83.h"


#include "camacdef.h"

SpinDAQ::SpinDAQ(QObject *parent) : QObject(parent)
{
    overall_trigger_count = 0;
    spill_count = 0;
    N_win = 0;

    session_log = new QFile("session_log.txt");
    session_log->open(QFile::WriteOnly);
    log_stream = new QTextStream(session_log);
    appendLogMessage("Start date/time: " + QDateTime::currentDateTime().toString("dd.MM.yy hh:mm"));

    qbus_line = new QbusLine(this);

    // menu options
     settings = new QSettings("/home/daq/project/daq_ilushin/build-daq_opt-Desktop_Qt_5_7_0_GCC_64bit-Debug/settings.conf",QSettings::NativeFormat);
     READ_MWPC1 =           settings->value("settings/MWPC1").toBool();
     READ_MWPC2 =           settings->value("settings/MWPC2").toBool();
     READ_VIS_SCALER =      settings->value("settings/VisualScaler").toBool();
     READ_SS =              settings->value("settings/SupplementaryScalers").toBool();
     READ_TOF2 =            settings->value("settings/TOF2").toBool();
     READ_CALORIMETER =     settings->value("settings/Calorimeter").toBool();
     output = true;

    //saveSettings();

    propcam_controller = new Le88(this);
    propcam_controller->setQbusLine(qbus_line);
    propcam_controller->setCrateAddress(8);

    sync_controller = new Le88(this);
    sync_controller->setQbusLine(qbus_line);
    sync_controller->setCrateAddress(4);

    counters_controller = new Le88(this);
    counters_controller->setQbusLine(qbus_line);
    counters_controller->setCrateAddress(9);

    // test modules
    sync_t162_19 = new ModuleT162(this);
    sync_t162_19->setModuleName("sync_t162_19");
    sync_t162_19->setQbusController(sync_controller);
    sync_t162_19->setStationNumber(21);

    counters_t162 = new ModuleT162(this);
    counters_t162->setModuleName("counters_t162");
    counters_t162->setQbusController(counters_controller);
    counters_t162->setStationNumber(15);

    // propcam
    br_rpk_1 = new ModuleBrrpk32(this);
    br_rpk_1->setModuleName("propcam_rpk1");
    br_rpk_1->setQbusController(sync_controller);
    br_rpk_1->setStationNumber(18);

    br_rpk_2 = new ModuleBrrpk32(this);
    br_rpk_2->setModuleName("propcam_rpk2");
    br_rpk_2->setQbusController(sync_controller);
    br_rpk_2->setStationNumber(20);


    kb_003 = new ModuleKb003(this);
    kb_003->setModuleName("kb_003");
    kb_003->setQbusController(propcam_controller);
    kb_003->setStationNumber(20);

    // tof
    le83 = new modulele83(this);
    le83->setQbusLine(qbus_line);
    le83->setCrateAddress(0);


    // counter
    vis_scaler = new ModuleCounter8(this);
    vis_scaler->setModuleName("vis_scaler");
    vis_scaler->setQbusController(counters_controller);
    vis_scaler->setStationNumber(21);

    s335_scaler = new ModuleS335(this);
    s335_scaler->setModuleName("s335_scaler");
    s335_scaler->setQbusController(sync_controller);
    s335_scaler->setStationNumber(12);

    tl2_scaler = new ModuleTl2(this);
    tl2_scaler->setModuleName("tl2_scaler");
    tl2_scaler->setQbusController(sync_controller);
    tl2_scaler->setStationNumber(14);

    calorimeter = new ModuleP267(this);
    calorimeter->setModuleName("calorimeter");
    calorimeter->setQbusController(counters_controller);
    calorimeter->setStationNumber(13);
}

SpinDAQ::~SpinDAQ(void)
{
    delete tl2_scaler;
    delete s335_scaler;
    delete vis_scaler;
    delete le83;
    delete kb_003;
    delete br_rpk_1;
    delete br_rpk_2;
    delete sync_t162_19;
    delete counters_t162;
    delete propcam_controller;
    delete sync_controller;
    delete counters_controller;
    delete qbus_line;
    delete calorimeter;

    //delete data_client;

    delete log_stream;
    session_log->close();
    delete session_log;
}

bool SpinDAQ::initialize(void)
{
    data_client = new DataClient();
    connect(data_client, SIGNAL(logMessage(QString)), this, SLOT(appendLogMessage(QString)));

    loadSettings();

    if(data_client->connectToServer())
    {
        appendLogMessage("Connected to data server");
    }
    else
    {
        return false;
    }

    return true;
}

void SpinDAQ::deinit(void)
{
    delete data_client;
}

bool SpinDAQ::startQbus(void)
{
    return qbus_line->start();
}

bool SpinDAQ::stopQbus(void)
{
    return qbus_line->stop();
}

bool SpinDAQ::sendData(void)
{
    if(output){
        appendLogMessage("sendData");
    }
    return (data_client->sendBS()
            && data_client->sendTR()
            && data_client->sendES()
            && data_client->sendPD());
}

void SpinDAQ::fillTestTriggers(int count)
{
    data_client->initTR();
    for(int n=0; n<count; n++)
    {
        data_client->addTR();

        uint16_t tof_data[TOF2_MAXIMUM_WORD_COUNT];
        int data_len = 30;
        for(int d=0; d<data_len; d++)
        {
            tof_data[d] = d;
        }
        data_client->appendTR("F2", tof_data, data_len);

        data_len = 50;
        uint16_t propcam_data[PROPCAM_MAXIMUM_WORD_COUNT];
        for(int d=0; d<data_len; d++)
            propcam_data[d] = d + 0x0100*n;
        data_client->appendTR("P1", propcam_data, data_len);

        for(int d=0; d<data_len; d++)
            propcam_data[d] = d + 0x1000*n;
        data_client->appendTR("P2", propcam_data, data_len);
    }
}

bool SpinDAQ::testCamac(void)
{
    propcam_controller->lamLRegister();
    propcam_controller->generateZ();
    propcam_controller->lamLRegister();
    propcam_controller->generateC();
    propcam_controller->lamLRegister();
    br_rpk_1->reset();
    propcam_controller->lamLRegister();
    br_rpk_1->startBRRPK();
    propcam_controller->lamLRegister();
    br_rpk_1->testLAM();
    propcam_controller->lamLRegister();
    br_rpk_1->testLAM();
    return br_rpk_1->testLAM();
}

bool SpinDAQ::Run_Setup_Camac(void)
{
    N_win++;
    if(output){
        appendLogMessage("sic: Run_Setup_Camac");
    }
    // check crates
    if(shakeT162(sync_t162_19)
            && shakeT162(counters_t162))
    {
        propcam_controller->generateZ();
        sync_controller->generateZ();
        counters_controller->generateZ();
//test
        propcam_controller->generateC();
        sync_controller->generateC();
        counters_controller->generateC();
//
        propcam_controller->lowerCrateInhibit();
        sync_controller->lowerCrateInhibit();
        counters_controller->lowerCrateInhibit();

        if(READ_TOF2==true){
            ClearTOF2();
            //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE84_RES);
        }
        //-----------""

        return true;
    }
    else
    {
        return false;
    }
}

void SpinDAQ::ClearTOF2(void)
{
    if(output){
        appendLogMessage("sic: ClearTOF2");
    }
    int n;
    le83->setRegim();

    //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE91_OS);
    //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE35_ZI);
    //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE91_STR);
/*
    for(n=0; n<TOF2_MAXIMUM_WORD_COUNT; n++)
    {
        //le35->readDataRegister();
        //if(!le35->qbusController()->statusQ())
        le83->readDataRegister();
        if(!le83->statusQ())
            break;
    }
    if(n >= TOF2_MAXIMUM_WORD_COUNT)
    {
        appendLogMessage("spindaq: ClearTOF2() error (too many words)");
    }
*/
}

void SpinDAQ::Begin_Spill_Setup_Camac(void)
{
    if(output){
        appendLogMessage("sic: Begin_Spill_Setup_Camac");
    }
    spill_trigger_count = 0;
    data_client->initTR();

    if(READ_TOF2==true){
        //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE84_RES);
        le83->ResetOS();         //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE91_OS);
        //sync_t162_17->outputPulse(SYNC_T162_17_CHANNEL_LE35_ZI);
    }
    if(READ_MWPC1==true){
        br_rpk_1->reset();
    }
    if(READ_MWPC2==true){
        br_rpk_2->reset();
    }
    //-----------""
    Unblock_Trigger_1();

    setT1Enable(1);     //kb_003->setOutput(KB003_MASK_ENABLE_T1);
}

void SpinDAQ::Unblock_Trigger_1(void)
{
    //appendLogMessage("sic: Unblock_Trigger_1");
    sync_t162_19->outputPulse(SYNC_T162_19_CHANNEL_UNLOCK_T1);
}

uint8_t SpinDAQ::Trigger_1_detected(void)
{
    return counters_controller->readClearIn1();
}

void SpinDAQ::fill_Trig1_record(void)
{
    N_win++;
    //appendLogMessage("sic: fill_Trig1_record");
    spill_trigger_count++;
    overall_trigger_count++;

    data_client->addTR();
    // append data to tr record

    if(READ_MWPC1==true && N_win==20){
        br_rpk_1->setTimeWindow(ModuleBrrpk32::TIME_WINDOW_ALL);
    }
    else if (READ_MWPC1==true) {
        br_rpk_1->setTimeWindow(ModuleBrrpk32::TIME_WINDOW_9);

    }
    if(READ_MWPC2==true && N_win==20){
        br_rpk_2->setTimeWindow(ModuleBrrpk32::TIME_WINDOW_ALL);
    }
    else if (READ_MWPC2==true) {
        br_rpk_2->setTimeWindow(ModuleBrrpk32::TIME_WINDOW_9);

    }
    //qDebug("W_WIN=%d\n",N_win);
    if (N_win==20) N_win=0;
    if(READ_MWPC1==true){
        br_rpk_1->startBRRPK();
    }
    if(READ_MWPC2==true){
        br_rpk_2->startBRRPK();
    }

    // TOF
    if(READ_TOF2==true){
        uint16_t tof_data[TOF2_MAXIMUM_WORD_COUNT];
        int data_len = Block_F8F0F8(TOF2_MAXIMUM_WORD_COUNT, tof_data);
        data_client->appendTR("F2", tof_data, data_len);
    }

    if(READ_MWPC1==true){
        uint16_t propcam_data[PROPCAM_MAXIMUM_WORD_COUNT];
        int data_len = Block_F8F2F27(br_rpk_1, PROPCAM_MAXIMUM_WORD_COUNT, propcam_data);
        data_client->appendTR("P1", propcam_data, data_len);
    }

    if(READ_MWPC2==true){
        uint16_t propcam_data[PROPCAM_MAXIMUM_WORD_COUNT];
        int data_len = Block_F8F2F27(br_rpk_2, PROPCAM_MAXIMUM_WORD_COUNT, propcam_data);
        data_client->appendTR("P2", propcam_data, data_len);
    }

    // Calorimeter
    if(READ_CALORIMETER==true){
        uint16_t ca_data[CALORIMETER_MAXIMUM_WORD_COUNT];
        for(int n=0;n<CALORIMETER_MAXIMUM_WORD_COUNT;n++){
            ca_data[n] = calorimeter->readMemory(n);
        }
        data_client->appendTR("CA",ca_data,CALORIMETER_MAXIMUM_WORD_COUNT);
    }

}
void SpinDAQ::fill_PD_record(void){
    data_client->initPD();

    data_client->addPD();
    sync_t162_19->outputPulse(SYNC_T162_19_CHANNEL_START_PD);
    // Calorimeter
    uint16_t ca_data[CALORIMETER_MAXIMUM_WORD_COUNT];
    for(int n=0;n<CALORIMETER_MAXIMUM_WORD_COUNT;n++){
        ca_data[n] = calorimeter->readMemory(n);
    }
    data_client->appendPD("CA",ca_data,CALORIMETER_MAXIMUM_WORD_COUNT);

}


bool SpinDAQ::End_Spill_Setup_Camac(void)
{
    if(output){
        appendLogMessage("sic: End_Spill_Setup_Camac");
    }
    setT1Enable(0);     //kb_003->setOutput(KB003_MASK_DISABLE_T1);

    if(shakeT162(sync_t162_19)
            && shakeT162(counters_t162))
    {

        if(READ_TOF2==true){
            ClearTOF2();
        }
        if(READ_CALORIMETER==true){
            fill_PD_record();
        }
        //-----------""
        return true;
    }
    else return false;
}

void SpinDAQ::fill_BS_record(void)
{
    if(output){
        appendLogMessage("sic: fill_BS_record");
    }
    // already filled
}

void SpinDAQ::fill_ES_record(void)
{
    uint32_t counter_data[8];
    if(output){
        appendLogMessage("sic: fill_ES_record");
    }
    spill_count++;
    data_client->initES();
    uint16_t spill_count_words[2] = {spill_count & 0xffff, (spill_count >> 16) & 0xffff};
    data_client->appendES("EA", spill_count_words, sizeof(spill_count) / 2);

    if(READ_VIS_SCALER==true){
        for(int n=0; n<8; n++)
        {
            counter_data[n] = vis_scaler->readCounter(n);
        }
        uint16_t counter_data_words[17];
        for(int n=0; n<8; n++)
        {
            counter_data_words[n * 2 + 1] = (counter_data[n] >> 16) & 0xffff;
            counter_data_words[n * 2] = counter_data[n] & 0xffff;
        }
        counter_data_words[16] = vis_scaler->readButtons();

        data_client->appendES("VS", counter_data_words, 17);
    }
    // todo
    if(READ_SS==true){
        uint16_t ss_data_words[8 + 16*2];

        s335_scaler->stopCounters();
        for(int n=0; n<8; n++)
        {
            ss_data_words[n] = s335_scaler->readCounter(n);
        }
        s335_scaler->resetCounters();

        for(int n=0; n<16; n++)
        {
            uint32_t counter_value = tl2_scaler->readCounter(n);
            ss_data_words[8 + n*2] = counter_value & 0xffff;
            ss_data_words[8 + n*2 + 1] = (counter_value >> 16) & 0xffff;
        }
        sync_t162_19->outputPulse(SYNC_T162_19_CHANNEL_RESET_TL2);
        data_client->appendES("SS", ss_data_words, 8 + 16*2);
    }
    //-----------""

    uint time = QDateTime::currentDateTime().toTime_t();
    uint16_t time_words[2] = {(time >> 16) & 0xffff, time & 0xffff};
    data_client->appendES("TM", time_words, sizeof(time) / 2);
}

int SpinDAQ::Block_F8F0F8(int max_no_words, uint16_t *buf)
{
    short dummy;
    int cnt_F8=0, max_cnt_F8=10000, nw=0;
    int no_block_error = 0;
    int Ncycle = 0;
    bool information = false;

    le83->setRegim();

        do{
            if( ++cnt_F8 > max_cnt_F8 ){
                qDebug("\n Error in Block_F8F0F8: no information from TOF.\n");
                information = le83->statusInformation();
                qDebug("informatiom: %d\n",information);
                return nw;
            }
        }while(!le83->statusInformation());

        information = le83->statusInformation();
        //while(le35->qbusController()->statusQ());
        if(information)
        {
            //appendLogMessage("information");
            if(le83->statusErrorRegime())  appendLogMessage("Error: OSH, regime error LE83");
            Ncycle = le83->getNcycle();
            qDebug("N cycl =  %d\n",Ncycle);
            for(int  i=0; i<Ncycle;i++){
                dummy = le83->readAddrRegister();
                buf[nw] = dummy;
                dummy = le83->readDataRegister();
                buf[nw+1] = dummy;
                nw = nw + 2;
                 //qDebug("%x",buf[nw]);

                if( nw > max_no_words )
                {
                    ++no_block_error;
                    qDebug("\nError in Block_F8F0F8: too many words.\n");
                    ClearTOF2();
                    return nw;
                }
            }

        }

    return nw;
}

int SpinDAQ::Block_F8F2F27(ModuleBrrpk32* module, int max_no_words, uint16_t *buf)
{
    short dummy;
    int cnt_F8=0, max_cnt_F8=10000, nw=0;
    int no_block_error = 0;

    do
    {
        do
        {
            if( ++cnt_F8 > max_cnt_F8 )
            {
                ++no_block_error;
                module->reset();
                qDebug("\n Error in Block_F8F2F27: no LAM.\n");
                return nw;
            }
        }
        while(!module->testLAM());

        //qDebug("LAM ok");

        dummy = module->readAndClearGroup1();
        while(module->qbusController()->statusQ())
        {
            buf[nw++]=dummy;
            if( nw > max_no_words )
            {
                ++no_block_error;
                module->reset();
                qDebug("\nError in Block_F8F2F27: too many words.\n");
                return nw;
            }
            dummy = module->readAndClearGroup1();
        }
    }
    while(module->testStatus());

    module->reset();
    return nw;
}

void SpinDAQ::appendLogMessage(QString string)
{
    qDebug(string.toLocal8Bit().constData());
    *log_stream << string + "\r\n";
    log_stream->flush();
}

bool SpinDAQ::shakeT162(ModuleT162* module)
{
    int n;
    uint16_t write_value;
    uint16_t read_value;
    int sh = 0;

    write_value = 0x0001;
    for(int k=0;k<2;k++){     //chek 2 times
        for(n=0; n<18; n++)
        {
           module->writeMemory(0, write_value);
           read_value = module->readMemory(0);
          if(read_value == write_value)
          {
             if(n < 15)
             {
                 write_value = write_value << 1;
             }
                else if(n == 15)
               {
                  write_value = 0xffff;
              }
              else
               {
                  write_value = 0x0000;
             }
             break;
           }
          else

          {   sh++;
               appendLogMessage(module->ModuleName() + ": shakeT162() error (write_value = 0x" + QString::number(write_value, 16) +
                                ", read_value = 0x" + QString::number(read_value, 16) + ")");
              if(sh==2)return false;
          }
      }
    }
    return true;
}

void SpinDAQ::setT1Enable(uint8_t enable)
{

    sync_controller->setOut3(enable);
}

void SpinDAQ::loadSettings(void)
{
    QFile xmlFile("settings.xml");
    xmlFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&xmlFile);

    QString level_name;

    while (!reader.atEnd())
    {
        reader.readNext();

        if(reader.tokenType() == QXmlStreamReader::StartElement)
        {
            level_name = reader.name().toString();
        }
        else if(reader.tokenType() == QXmlStreamReader::EndElement) level_name = "";
        else if(reader.tokenType() == QXmlStreamReader::Characters)
        {
            if(level_name == "data_server_host")
            {
                data_client->server_host.setAddress(reader.text().toString());
            }
            else if(level_name == "data_server_port")
            {
                data_client->server_port = reader.text().toInt();
            }
            else if(level_name == "connection_mode")
            {
                if(reader.text().toString() == "remote")
                {
                    qbus_line->setConnectionMode(QbusLine::CONNECTION_MODE_REMOTE);
                }
                else
                {
                    qbus_line->setConnectionMode(QbusLine::CONNECTION_MODE_LOCAL);
                }
            }
        }
    }
}

void SpinDAQ::saveSettings(void)
{
    QFile xmlFile("settings.xml");
    xmlFile.open(QIODevice::WriteOnly);

    QXmlStreamWriter writer(&xmlFile);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("settings");

    writer.writeTextElement("data_server_host", data_client->server_host.toString());
    writer.writeTextElement("data_server_port", QString::number(data_client->server_port));
    if(qbus_line->connectionMode() == QbusLine::CONNECTION_MODE_REMOTE)
    {
        writer.writeTextElement("connection_mode", "remote");
    }
    else
    {
        writer.writeTextElement("connection_mode", "local");
    }

    writer.writeEndElement();	//settings
    writer.writeEndDocument();
}
