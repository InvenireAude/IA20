/*
 * File: Socket.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Conn_MCast_Socket_H_
#define _IA20_Net_Conn_MCast_Socket_H_

#include <ia20/commonlib/commonlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../Address.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace MCast {

/*************************************************************************/
/** The Socket class.
 *
 */
class Socket {
public:

	virtual ~Socket() throw();

	Socket(const String& strInterface, const String& strGroup, uint16_t iGroupPort);

  int send(const void* pData, uint32_t iDataLen);
  bool receive(void *pData, uint32_t& iDataSize, Address& address);

  static const uint32_t CMaxPacketSize = 1500;

  Socket(const Socket&) = delete;

protected:

  String strInterface;
  String strGroup;
  uint16_t iPort;

  struct sockaddr_in group_sock;
  int fd;

  int iTimeout;
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Conn_MCast_Socket_H_ */
