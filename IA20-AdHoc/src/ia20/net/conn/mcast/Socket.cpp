/*
 * File: Socket.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Socket.h"

#include <unistd.h>
#include <stdio.h>

// MVSC feels better with this
#ifndef __USE_MISC
#define __USE_MISC
#endif

#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>


namespace IA20 {
namespace Net {
namespace Conn {
namespace MCast {

/*************************************************************************/
Socket::Socket(const String& strInterface, const String& strGroup, uint16_t iGroupPort):
  strInterface(strInterface),
  strGroup(strGroup),
  iPort(iPort),
  iTimeout(120000){
	IA20_TRACER;

  if((fd=socket(AF_INET, SOCK_DGRAM, 0)) < 0){
   IA20_THROW(SystemException("Cannot create socket.",errno));
  }

  struct in_addr local_interface;
  local_interface.s_addr = inet_addr(strInterface.c_str());

  if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&local_interface, sizeof(local_interface)) < 0){
      IA20_THROW(SystemException("Cannot set IP_MULTICAST_IF on socket.",errno));
  }


	group_sock.sin_family = AF_INET;
  group_sock.sin_addr.s_addr = inet_addr(strGroup.c_str());
  group_sock.sin_port = htons(iGroupPort);


	// int size = 1000 * 1024 * 1024;
	// if (setsockopt(iSocket, SOL_SOCKET, SO_RCVBUF, &size, (socklen_t)sizeof(int)) < 0)
    // 	IA20_THROW(SystemException()<<"UDP mulitcast socket (SO_RCVBUF), port: "<<(int)iPort);

	int reuse = 1;

  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0){
      IA20_THROW(SystemException("Cannot set SO_REUSEADDR on socket.",errno));
  }

	struct sockaddr_in local_sock;

  bzero((char *)&local_sock, sizeof(local_sock));

	local_sock.sin_family = AF_INET;
  local_sock.sin_port = htons(iGroupPort);
  local_sock.sin_addr.s_addr = INADDR_ANY;

  if(bind(fd, (struct sockaddr *)&local_sock, sizeof(local_sock)) == -1){
		 IA20_THROW(SystemException("Cannot bind to socket.",errno));
  }


	struct ip_mreq group;

	group.imr_multiaddr.s_addr = inet_addr(strGroup.c_str());
  group.imr_interface.s_addr = inet_addr(strInterface.c_str());

  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0){
	 IA20_THROW(SystemException("Cannot add membership to socket.",errno));
	}

  IA20_LOG(LogLevel::INSTANCE.isSystem(),"Socket"<<fd<<", group: "<<strGroup.c_str()<<"["<<iGroupPort<<"], on: "<<strInterface);

}
/**********************************************************************/
int Socket::send(const void* pData, uint32_t iDataLen){

	int result = sendto(fd, pData, iDataLen, 0,
                      (struct sockaddr *)&group_sock, sizeof(group_sock));

	if(result < 0){
	  IA20_THROW(SystemException("Cannot send to socket.",errno));
	}

  IA20_LOG(LogLevel::INSTANCE.isSystem(),"Socket ["<<fd<<"] is sending "<<iDataLen<<" bytes.");

  return 0;
}
/**********************************************************************/
bool Socket::receive(void *pData, uint32_t& iDataSize, Address& address){

	IA20_TRACER;

	int32_t iResult = 0;

	{
		Thread::Cancellation ca(true);
    // IA20_LOG(LogLevel::INSTANCE.isInfo(),"Timeout: "<<iTimeout);
		// if(iTimeout != 0)
		// 	if(!waitForData(WM_Read)){
		// 		iDataSize = 0;
		// 		return false;
		// }


		iResult = ::recvfrom(fd, pData, CMaxPacketSize, 0, (struct sockaddr*)&address.address, &address.iAddressLen);
	}

  IA20_LOG(LogLevel::INSTANCE.isSystem(),"Received "<<iResult<<" bytes from: "<<address);

  // TODO class ipAddress or at least conversions.

	if(iResult < 0){
		IA20_LOG(LogLevel::INSTANCE.isError(),"READ:"<<(void*)pData<<"len="<<iResult);
		IA20_THROW(SystemException("read",errno));
	}

  IA20_LOG(LogLevel::INSTANCE.isData(), "READ: "<<MiscTools::BinarytoHex((unsigned char*)pData,iResult));

	iDataSize = iResult;
  return true;
}
/*************************************************************************/
Socket::~Socket() throw(){
	IA20_TRACER;

 if(fd< 0)
  return;

  close(fd);
}
/*************************************************************************/
}
}
}
}
