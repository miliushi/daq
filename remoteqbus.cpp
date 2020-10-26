#include "remoteqbus.h"

RemoteQbus::RemoteQbus(QObject *parent) : QObject(parent)
{
    ssh_socket = new QSshSocket(this);

    ssh_socket->connectToHost("neppc9.ihep.su");
    while(!ssh_socket->isConnected())
    {
        //qDebug(".");
    }
    qDebug("connected");
    ssh_socket->login("daq", "1BV110");
    while(!ssh_socket->isLoggedIn())
    {
        //qDebug("!");
    }
    qDebug("logged in");

    setDevice("/dev/qbus0");
}

RemoteQbus::~RemoteQbus(void)
{
    delete ssh_socket;
}

bool RemoteQbus::start(void)
{
/*    int rc;

    ssh_verbosity = SSH_LOG_PROTOCOL;
    ssh_port = 22;

    ssh_options_set(qbus_ssh_session, SSH_OPTIONS_HOST, host_name.toLocal8Bit().constData());
    ssh_options_set(qbus_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &ssh_verbosity);
    ssh_options_set(qbus_ssh_session, SSH_OPTIONS_PORT, &ssh_port);

    rc = ssh_connect(qbus_ssh_session);
    if(rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to host: %s\n",
              ssh_get_error(qbus_ssh_session));
        return false;
    }

    if (verifyKnownhost(qbus_ssh_session) < 0)
    {
        ssh_disconnect(qbus_ssh_session);
        ssh_free(qbus_ssh_session);
        return false;
    }

    // authenticate
    //password = getpass("Password: ");
    rc = ssh_userauth_password(qbus_ssh_session, NULL, password.toLocal8Bit().constData());
    if (rc != SSH_AUTH_SUCCESS)
    {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(qbus_ssh_session));
        ssh_disconnect(qbus_ssh_session);
        //ssh_free(qbus_ssh_session);
        return false;
    }

    qDebug("ssh is connected");*/
    return true;
}

bool RemoteQbus::stop(void)
{
 /*   ssh_disconnect(qbus_ssh_session);
    //ssh_free(qbus_ssh_session);

    qDebug("ssh is disconnected");*/
    return true;
}

void RemoteQbus::setDevice(QString device)
{
    qbus_device = device;
}

bool RemoteQbus::writeRegister(uint16_t addr, uint16_t value)
{
    QString reply;

    reply = executeQbusCommand("write " + QString::number(addr) + " " + QString::number(value));

    if(reply.startsWith("write "))
        return true;
    else
        return false;
}

bool RemoteQbus::readRegister(uint16_t addr, uint16_t* value)
{
    QString reply;

    reply = executeQbusCommand("read " + QString::number(addr));

    if(reply.startsWith("read "))
    {
        reply.remove(0, reply.indexOf('=') + 1);
        //qDebug(reply.toLocal8Bit().data());
        *value = reply.toInt(nullptr, 16);
        //qDebug(QString::number(*value).toLocal8Bit().data());
        return true;
    }
    else
        return false;
}

QString RemoteQbus::executeQbusCommand(QString command)
{
    command_executed = false;
    ssh_socket->executeCommand("./apps/qbus/qbus " + qbus_device + " " + command);
    while(!ssh_socket->isExecuted());

    return ssh_socket->response.toLocal8Bit().data();
}

/*
int RemoteQbus::verifyKnownhost(ssh_session session)
{
  int state;
  int hlen;
  unsigned char *hash = NULL;
  char *hexa;
  char buf[10];
  state = ssh_is_server_known(session);
  hlen = ssh_get_pubkey_hash(session, &hash);
  if (hlen < 0)
    return -1;
  switch (state)
  {
    case SSH_SERVER_KNOWN_OK:
      break;
    case SSH_SERVER_KNOWN_CHANGED:
      fprintf(stderr, "Host key for server changed: it is now:\n");
      ssh_print_hexa("Public key hash", hash, hlen);
      fprintf(stderr, "For security reasons, connection will be stopped\n");
      free(hash);
      return -1;
    case SSH_SERVER_FOUND_OTHER:
      fprintf(stderr, "The host key for this server was not found but an other"
        "type of key exists.\n");
      fprintf(stderr, "An attacker might change the default server key to"
        "confuse your client into thinking the key does not exist\n");
      free(hash);
      return -1;
    case SSH_SERVER_FILE_NOT_FOUND:
      fprintf(stderr, "Could not find known host file.\n");
      fprintf(stderr, "If you accept the host key here, the file will be"
       "automatically created.\n");
      // fallback to SSH_SERVER_NOT_KNOWN behavior
    case SSH_SERVER_NOT_KNOWN:
      hexa = ssh_get_hexa(hash, hlen);
      fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
      fprintf(stderr, "Public key hash: %s\n", hexa);
      free(hexa);
      if (fgets(buf, sizeof(buf), stdin) == NULL)
      {
        free(hash);
        return -1;
      }
      if (strncasecmp(buf, "yes", 3) != 0)
      {
        free(hash);
        return -1;
      }
      if (ssh_write_knownhost(session) < 0)
      {
        fprintf(stderr, "Error %s\n", strerror(errno));
        free(hash);
        return -1;
      }
      break;
    case SSH_SERVER_ERROR:
      fprintf(stderr, "Error %s", ssh_get_error(session));
      free(hash);
      return -1;
  }
  free(hash);
  return 0;
}

int RemoteQbus::executeCommand(QString command)
{
    ssh_channel channel;
    int rc;
    char buffer[256];
    int nbytes;

    channel = ssh_channel_new(qbus_ssh_session);
    if (channel == NULL)
      return SSH_ERROR;

    //qDebug(QString("channel=" + QString::number(channel->local_channel)).toLocal8Bit().constData());

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK)
    {
      ssh_channel_free(channel);
      return rc;
    }
/*
    rc = ssh_channel_request_exec(channel, "ls");
    if (rc != SSH_OK)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return rc;
    }
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0)
    {
      if (write(1, buffer, nbytes) != (unsigned int) nbytes)
      {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return SSH_ERROR;
      }
      nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0)
    {
      ssh_channel_close(channel);
      ssh_channel_free(channel);
      return SSH_ERROR;
    }
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    fprintf(stderr, buffer);

    return SSH_OK;
}*/
/*
void RemoteQbus::commandExecuted(QString command,QString response)
{
    qDebug(response.toLocal8Bit().constData());
    command_executed = true;
}

void RemoteQbus::sshError(QSshSocket::SshError)
{
    qDebug("error");
    command_executed = true;
}

void RemoteQbus::test(void)
{
    qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}*/
