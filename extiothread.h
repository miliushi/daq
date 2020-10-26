#ifndef EXTIOTHREAD_H
#define EXTIOTHREAD_H

#include <QThread>
#include <stdint.h>
class ExtIO;

class ExtIOThread : public QThread
{
    Q_OBJECT
public:
    ExtIOThread();
    ~ExtIOThread(void);
protected:
    void run();
signals:
    void gateBegin(void);
    void gateEnd(void);

    void logMessage(QString);
    void fatalError(void);

    void startDaq(void);
    void stopDaq(void);
public slots:

private:
    void readControl(void);
    bool abort;

    ExtIO* ext_io;

    uint8_t gate_state;
};

#endif // EXTIOTHREAD_H
