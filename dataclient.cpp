#include "dataclient.h"
#include <QDataStream>

DataClient::DataClient(QObject *parent) : QObject(parent)
{
    first_packet_flag = true;
    connection_timeout = 5 * 1000;
    tcp_socket = new QTcpSocket(this);
}

DataClient::~DataClient(void)
{
    delete tcp_socket;
}

bool DataClient::connectToServer(void)
{
    tcp_socket->connectToHost(server_host, server_port);
    if (tcp_socket->waitForConnected(connection_timeout))
    {
        return true;
    }
    else
    {
        emit logMessage("Data server connection error: " + QString::number(tcp_socket->error()) + " (" + tcp_socket->errorString() + ")");
        return false;
    }
}

void DataClient::initTR(void)
{
    tr_records.clear();
}

void DataClient::addTR(void)
{
    const char chardata[] = {'?', '?', 0, 2, 0, 1, '?', '?', 'T', 'R'};
    const QByteArray record = QByteArray(chardata, sizeof(chardata));

    tr_records.append(record);
}

void DataClient::initES(void)
{
    es_record.clear();
    es_record.append("??", 2);
    // append char
    es_record.append((char)0);
    es_record.append((char)2);     // type
    es_record.append((char)0);
    es_record.append((char)1);     // count
    //
    es_record.append("??", 2);
    es_record.append("ES", 2);
}

void DataClient::appendTR(const char* block_name, uint16_t* data, int data_size)
{
    uint16_t block_length = 2 + (data_size);
    tr_records.last().append((block_length >> 8) & 0xff);
    tr_records.last().append(block_length & 0xff);
    tr_records.last().append(block_name, 2);
    // swap bytes
    for(int n=0; n<data_size; n++)
    {
        tr_records.last().append((data[n] >> 8) & 0x00ff);
        tr_records.last().append(data[n] & 0x00ff);
    }
}

void DataClient::appendES(const char* block_name, uint16_t* data, int data_size)
{
    uint16_t block_length = 2 + (data_size);
    es_record.append((block_length >> 8) & 0xff);
    es_record.append(block_length & 0xff);
    es_record.append(block_name, 2);
    // swap bytes
    for(int n=0; n<data_size; n++)
    {
        es_record.append((data[n] >> 8) & 0xff);
        es_record.append(data[n] & 0xff);
    }
}

bool DataClient::sendBS(void)
{
    const char chardata[] = {0, 2, 0, 2, 0, 1, 0, 2, 'B', 'S'};
    QByteArray record = QByteArray(chardata, sizeof(chardata));

    if(first_packet_flag)
    {
        record[3] = 1;
        first_packet_flag = false;
    }

    return writeData(record);
}

bool DataClient::sendTR(void)
{
    bool return_value = true;
    // set record size
    for(int n=0; n<tr_records.count(); n++)
    {
        tr_records[n][0] = (((tr_records.at(n).size() / 2) - 3) >> 8) & 0xff;
        tr_records[n][1] = ((tr_records.at(n).size() / 2) - 3) & 0xff;
        tr_records[n][6] = tr_records.at(n)[0];
        tr_records[n][7] = tr_records.at(n)[1];
        return_value &= writeData(tr_records.at(n));
    }

    return return_value;
}

bool DataClient::sendES(void)
{
    // set record size
    es_record[0] = (((es_record.size() / 2) - 3) >> 8) & 0xff;
    es_record[1] = ((es_record.size() / 2) - 3) & 0xff;
    es_record[6] = es_record[0];
    es_record[7] = es_record[1];

    return writeData(es_record);
}

bool DataClient::writeData(QByteArray data)
{
    if(tcp_socket->state() == QAbstractSocket::ConnectedState)
    {
        tcp_socket->write(data); //write the data itself
        return tcp_socket->waitForBytesWritten();
    }
    else
        return false;
}

