/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/Receiver.cpp
 *
 * Copyright (C) 2020, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Receiver.h"

#include <ia20/commonlib/commonlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace IA20 {
namespace Net {
namespace Conn {
namespace MCast {

/*************************************************************************/
Receiver::Receiver(unsigned int iPort):
	Base(iPort){
	IA20_TRACER;

	// int size = 1000 * 1024 * 1024;

	// if (setsockopt(iSocket, SOL_SOCKET, SO_RCVBUF, &size, (socklen_t)sizeof(int)) < 0)
    // 	IA20_THROW(SystemException()<<"UDP mulitcast socket (SO_RCVBUF), port: "<<(int)iPort);

	int reuse = 1;
    if (setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    	IA20_THROW(SystemException()<<"UDP mulitcast socket (SO_REUSEADDR), port: "<<(int)iPort);
}
/*************************************************************************/
Receiver::~Receiver() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Receiver::bind(){
	IA20_TRACER;
	struct sockaddr_in localSock;

	bzero((char *)&localSock, sizeof(localSock));

	localSock.sin_family = AF_INET;
    localSock.sin_port = htons(iPort);
    localSock.sin_addr.s_addr = INADDR_ANY;

    if(::bind(iSocket, (struct sockaddr *)&localSock, sizeof(localSock)) == -1){
		IA20_THROW(SystemException()<<"UDP mulitcast socket (bind), port: "<<(int)iPort);
    }

}
/*************************************************************************/
void Receiver::subscribe(const String& strInterface, const String& strGroup){
	IA20_TRACER;

	struct ip_mreq group;

	group.imr_multiaddr.s_addr = inet_addr(strGroup.c_str());
    group.imr_interface.s_addr = inet_addr(strInterface.c_str());

    if (setsockopt(iSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0){
		IA20_THROW(SystemException()<<"UDP mulitcast socket (IP_ADD_MEMBERSHIP), interface: "<<
			strInterface<<", group: "<<strGroup);
	}

}
/*************************************************************************/
bool Receiver::receive(void *pData, size_t iBufferLen, Address& address){
	IA20_TRACER;

	ssize_t iResult = 0;

	{
		Thread::Cancellation ca(true);
		if(iTimeout != 0)
			if(!waitForData(WM_Read)){
				return false;
		}

	  	iResult = ::recvfrom(iSocket, pData, CMaxPacketSize, 0, (struct sockaddr*)&address.address, &address.iAddressLen);
	}

	if(iResult < 0){
		IA20_LOG(LogLevel::INSTANCE.isError(),"READ:"<<(void*)pData<<"len="<<iBufferLen);
		IA20_THROW(SystemException("read",errno));
	}

	if(LogLevel::INSTANCE.isData()){
		String strData;
		for(int i=0;i<iResult;i++)
			strData += String("") + (*(((char*)pData)+i) >= ' ' ? *(((char*)pData)+i) : '.');
		IA20_LOG(LogLevel::INSTANCE.isData(),"READ:"<<iResult<<" ["<<strData<<"]");
	}

	return true;
}
/*************************************************************************/
}
}
}
}