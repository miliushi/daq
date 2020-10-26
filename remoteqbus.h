#ifndef REMOTEQBUS_H
#define REMOTEQBUS_H

#include <QObject>
#include <libssh/libssh.h>
#include "qsshsocket.h"

class RemoteQbus : public QObject
{
    Q_OBJECT
public:
    explicit RemoteQbus(QObject *parent = 0);
    ~RemoteQbus(void);

    bool start(void);
    bool stop(void);

    void setDevice(QString device);
    bool writeRegister(uint16_t addr, uint16_t value);
    bool readRegister(uint16_t addr, uint16_t* value);

signals:
public slots:
private slots:
    //void commandExecuted(QString command,QString response);
    //void sshError(QSshSocket::SshError);
private:
    QString executeQbusCommand(QString command);
    //int verifyKnownhost(ssh_session session);
    //int executeCommand(QString command);

    QSshSocket* ssh_socket;

    ssh_session qbus_ssh_session;
    int ssh_verbosity = SSH_LOG_PROTOCOL;
    int ssh_port = 22;

    QString host_name;
    QString password;

    bool command_executed;
    bool is_connected;

    //-------
    QString qbus_device;
};

#endif // REMOTEQBUS_H
