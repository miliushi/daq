#ifndef MODULEBRRPK32_H
#define MODULEBRRPK32_H

#include <stdint.h>
#include "qbusmodule.h"

class ModuleBrrpk32 : public QbusModule
{
    Q_OBJECT
public:
    ModuleBrrpk32(QObject *parent = 0);

    typedef enum
    {
        TIME_WINDOW_ALL = 0x0f,
        TIME_WINDOW_0 = 0x10,
        TIME_WINDOW_1 = 0x11,
        TIME_WINDOW_2 = 0x12,
        TIME_WINDOW_3 = 0x13,
        TIME_WINDOW_4 = 0x14,
        TIME_WINDOW_5 = 0x15,
        TIME_WINDOW_6 = 0x16,
        TIME_WINDOW_7 = 0x17,
        TIME_WINDOW_8 = 0x18,
        TIME_WINDOW_9 = 0x19,
        TIME_WINDOW_10 = 0x1a,
        TIME_WINDOW_11 = 0x1b,
        TIME_WINDOW_12 = 0x1c,
        TIME_WINDOW_13 = 0x1d,
        TIME_WINDOW_14 = 0x1e,
        TIME_WINDOW_15 = 0x1f
    }TimeWindow;

    void reset(void);
    void setTimeWindow(TimeWindow value);
    void startBRRPK(void);
    uint16_t readAndClearGroup1(void);
    bool testStatus(void);
};

#endif // MODULEBRRPK32_H
