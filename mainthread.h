#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
class SpinDAQ;
class ExtIOThread;

class MainThread : public QThread
{
    Q_OBJECT
public:
    MainThread();
    ~MainThread(void);

    QString xml_out;
protected:
    void run();
signals:
    void finished();
public slots:

private slots:
    void gateBegin(void);
    void gateEnd(void);
    void extioFatalError(void);
    void startDaq(void);
    void stopDaq(void);
private:
    void waitGateBegin(void);
    void waitGateEnd(void);

    bool abort;
    bool daq_is_running;

    bool gate_begin_flag;
    bool gate_end_flag;

    SpinDAQ* spin_daq;
    ExtIOThread* ext_io_thread;
};

#endif // MAINTHREAD_H
