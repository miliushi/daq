// qbus.h - PC QBUS card device 
//
// Mar-2004 by Solo
// Apr-2005 by Solo - moved from std:: namespace to global(::)
// Nov-2007 by Solo - added device name query

#ifndef _QBUS_H
#define _QBUS_H

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

//#define QBUS_USE_IOCTL

#ifdef QBUS_USE_IOCTL
#include <stropts.h>
#include <linux/ioctl.h>
// IOCTLs
#define QBUS_IOC_MAGIC 'Q'
#define QBUS_IOCTADDR  _IO  (QBUS_IOC_MAGIC, 0)
#define QBUS_IOCCLEAR  _IO  (QBUS_IOC_MAGIC, 1)
#define QBUS_IOCRESET  _IO  (QBUS_IOC_MAGIC, 2)
#define QBUS_IOCXREAD  _IOWR(QBUS_IOC_MAGIC, 3, int)
#define QBUS_IOCSWRITE _IOW (QBUS_IOC_MAGIC, 4, int)
#endif

class QBUS {

 protected:
  int m_node;
  char *m_device;

 public:
  // constructor
  // QBUS() {}
  QBUS(char *device_name) {
    m_device=NULL;
    m_node=::open(device_name,O_RDWR);
    if(m_node<0) {
      printf("QBUS::Error opening device: %s - %s\n",device_name,strerror(errno));
    }
    m_device=strdup(device_name);
  }

  // destructor
  ~QBUS() { 
    if(m_node>=0) ::close(m_node); 
    if(m_device!=NULL) delete m_device;
  }

  // get minor node number
  int node() { 
    return m_node; 
  } 

  // get device name
  char * device() {
    return strdup(m_device);
  }

  // set address
  void lseek(int addr,int whence) {
#ifdef QBUS_USE_IOCTL
    int a = addr;
    ioctl(m_node, QBUS_IOCTADDR, &a);
#else
    ::lseek(m_node, addr, whence);
#endif
  }

  // clear error status
  void clear() {
#ifdef QBUS_USE_IOCTL
    ioctl(m_node, QBUS_IOCCLEAR);
#else
    ::lseek(m_node, 0, 1);
#endif
  }

  // reset branch
  void reset() { 
#ifdef QBUS_USE_IOCTL
    ioctl(m_node, QBUS_IOCRESET);
#else
    ::lseek(m_node, 0, 2);
#endif 
  }

  // read one word
  int read(unsigned int addr) { 
    unsigned int data=0;
#ifdef QBUS_USE_IOCTL
    data=addr;
    ioctl(m_node,QBUS_IOCXREAD,&data);
    data=data&0xffff;
#else
    ::lseek(m_node, addr, 0);
    ::read(m_node,&data,2);
#endif
    return data;
  }

  // write one word
  int write(unsigned int addr, unsigned int data) {
#ifdef QBUS_USE_IOCTL
    unsigned int d=(addr<<16)|data;
    ioctl(m_node,QBUS_IOCSWRITE,&d);
    return 2;
#else
    ::lseek(m_node, addr, 0);
    return ::write(m_node,&data,2);
#endif
  }

};

#endif
