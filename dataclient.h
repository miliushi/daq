#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>

class DataClient : public QObject
{
    Q_OBJECT
public:
    explicit DataClient(QObject *parent = 0);
    ~DataClient(void);

    bool connectToServer(void);

    void initTR(void);  // clear TR records list
    void addTR(void);   // append new TR record
    void initES(void);  // clear ES record data

    void appendTR(const char* block_name, uint16_t* data, int data_size); // data size in words
    void appendES(const char* block_name, uint16_t* data, int data_size); // data size in words

    bool sendBS(void);
    bool sendTR(void);
    bool sendES(void);

    QHostAddress server_host;
    int server_port;
    int connection_timeout;
signals:
    void logMessage(QString);
public slots:
private:
    bool writeData(QByteArray data);

    QTcpSocket* tcp_socket;

    //QByteArray tr_record;
    QByteArrayList tr_records;
    QByteArray es_record;

    bool first_packet_flag;
};

#endif // DATACLIENT_H
