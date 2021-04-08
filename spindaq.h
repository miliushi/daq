#ifndef SPINDAQ_H
#define SPINDAQ_H

#include <QObject>
#include <QSettings>

#define SYNC_T162_17_CHANNEL_LE91_OS        0
#define SYNC_T162_17_CHANNEL_LE35_ZI        1
#define SYNC_T162_17_CHANNEL_LE84_RES       2
#define SYNC_T162_17_CHANNEL_LE91_STR       3

#define SYNC_T162_19_CHANNEL_RESET_TL2      0
#define SYNC_T162_19_CHANNEL_UNLOCK_T1      1
#define SYNC_T162_19_CHANNEL_START_PD       2

#define TOF2_MAXIMUM_WORD_COUNT             2192
#define PROPCAM_MAXIMUM_WORD_COUNT          849
#define CALORIMETER_MAXIMUM_WORD_COUNT      1

#define KB003_MASK_ENABLE_T1                0x02
#define KB003_MASK_DISABLE_T1               0x00

class QFile;
class QTextStream;

class DataClient;

class QbusLine;
class Le88;
class modulele83;
class ModuleT162;
class ModuleBrrpk32;
class ModuleLe35;
class ModuleKb003;
class ModuleCounter8;
class ModuleS335;
class ModuleTl2;
class ModuleP267;


class SpinDAQ : public QObject
{
    Q_OBJECT
public:
    explicit SpinDAQ(QObject *parent = 0);
    ~SpinDAQ(void);

    bool initialize(void);
    void deinit(void);

    bool startQbus(void);
    bool stopQbus(void);

    bool sendData(void);

    void fillTestTriggers(int count);
    bool testCamac(void);

    //chuiko
    bool Run_Setup_Camac(void);
    void ClearTOF2(void);
    void Begin_Spill_Setup_Camac(void);
    void Unblock_Trigger_1(void);
    uint8_t Trigger_1_detected(void);
    void fill_Trig1_record(void);
    bool End_Spill_Setup_Camac(void);
    void fill_BS_record(void);
    void fill_ES_record(void);
    void fill_PD_record(void);
    int Block_F8F0F8(int max_no_words, uint16_t *buf );
    int Block_F8F2F27(ModuleBrrpk32* module, int max_no_words, uint16_t *buf);


    // test
    //void setT1Enable(uint8_t enable);
    //
signals:

public slots:
    void appendLogMessage(QString string);
private slots:

private:
    bool READ_MWPC1;
    bool READ_MWPC2;
    bool READ_VIS_SCALER;
    bool READ_SS;
    bool READ_CALORIMETER;
    bool READ_TOF2;
    bool output; // terminal out
    bool TOF_DATA;

    int N_win;

    bool shakeT162(ModuleT162* module);
    void setT1Enable(uint8_t enable);

    void loadSettings(void);
    void saveSettings(void);

    int spill_trigger_count_max;
    int overall_trigger_count_max;

    int spill_trigger_count;
    int overall_trigger_count;
    uint32_t spill_count;

    QFile* session_log;
    QTextStream* log_stream;

    DataClient* data_client;


    QbusLine* qbus_line;

    Le88* propcam_controller;
    Le88* sync_controller;
    Le88* counters_controller;

    //Le83* tof_controller;

    ModuleT162* propcam_t162;
    ModuleT162* sync_t162_19;
    ModuleT162* counters_t162;

    ModuleBrrpk32* br_rpk_1;
    ModuleBrrpk32* br_rpk_2;
    ModuleKb003* kb_003;

    ModuleT162* sync_t162_17;

    ModuleLe35* le35;
    modulele83* le83;

    ModuleCounter8* vis_scaler;
    ModuleS335* s335_scaler;
    ModuleTl2* tl2_scaler;
    ModuleP267* calorimeter;

    QSettings *settings;
};

#endif // SPINDAQ_H
