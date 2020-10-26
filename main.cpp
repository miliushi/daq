#include <QApplication>
#include "mainthread.h"

int main(int argc, char *argv[])
{
    MainThread main_thread;

    QApplication a(argc, argv);

    QObject::connect(&main_thread, SIGNAL(finished()), &a, SLOT(quit()));

    main_thread.start(QThread::NormalPriority);
    return a.exec();
}
