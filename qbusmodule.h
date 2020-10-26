#ifndef QBUSMODULE_H
#define QBUSMODULE_H

#include <QObject>
class QbusController;

class QbusModule : public QObject
{
    Q_OBJECT
public:
    explicit QbusModule(QObject *parent = 0);

    void setQbusController(QbusController* controller);
    QbusController* qbusController(void);

    void setStationNumber(int number);
    int stationNumber(void);
    void setModuleName(QString name);
    QString ModuleName(void);

    bool testLAM(void);

signals:

public slots:
private:
    QbusController* qbus_controller;
    int station_number;
    QString module_name;

};

#endif // QBUSMODULE_H
