/*
 * File: IA20-CommonLib/src/commonlib/net/Client.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
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

#include <ia20/commonlib/commonlib.h>

#include "Client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include "PeerAddrInfo.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
Client::Client(const Peer& peerRemote, ConnectionFactory *pConnectionFactory):
  Connection(pConnectionFactory),
	peerRemote(peerRemote){}
/*************************************************************************/
Client::Client(const Peer& peerRemote, const Peer& peerLocal, ConnectionFactory *pConnectionFactory):
	Connection(peerLocal, pConnectionFactory),
	peerRemote(peerRemote){}
/*************************************************************************/
Client::~Client() throw(){
	IA20_TRACER;
}
/*************************************************************************/
FileHandle* Client::connect()const{
	IA20_TRACER;

	PeerAddrInfo aiLocal(peerLocal);
	PeerAddrInfo aiRemote(peerRemote);


	for(PeerAddrInfo::const_iterator itLocal=aiLocal.begin(); itLocal != aiLocal.end(); ++itLocal)
		for(PeerAddrInfo::const_iterator itRemote=aiRemote.begin(); itRemote != aiRemote.end(); ++itRemote){

		int iSocket;

		if((iSocket = socket((*itLocal)->ai_family, (*itLocal)->ai_socktype, (*itLocal)->ai_protocol)) < 0)
			IA20_THROW(SystemException("socket(): ")<<peerLocal);

		IA20_LOG(LogLevel::INSTANCE.isInfo(),"Local: "<<peerLocal<<", socket fd :"<<iSocket);

		IA20_LOG(LogLevel::INSTANCE.isInfo(),"Client: "<<iSocket<<", addr="
					<<inet_ntoa(((struct sockaddr_in*)((*itLocal)->ai_addr))->sin_addr)
					<<", port="<<(ntohs(((struct sockaddr_in*)((*itLocal)->ai_addr))->sin_port))
					<<", family="<<((struct sockaddr_in*)((*itLocal)->ai_addr))->sin_family);

	  int reuse = 1;

    if (setsockopt(iSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
    	IA20_THROW(SystemException()<<" socket (SO_REUSEADDR), port: ");

   // if (setsockopt(iSocket, SOL_SOCKET, SO_REUSEPORT, (char *)&reuse, sizeof(reuse)) < 0)
   // 	IA20_THROW(SystemException()<<"socket (SO_REUSEPORT), port: ");

		if(::bind(iSocket, (*itLocal)->ai_addr, (*itLocal)->ai_addrlen) == 0){

			IA20_LOG(LogLevel::INSTANCE.isInfo(),"Remote: "<<peerRemote);

			if(::connect(iSocket, (*itRemote)->ai_addr, (*itRemote)->ai_addrlen) == 0){

				if(bNoDelay)
	 				setNoDelayImpl(iSocket, 1);

			  IA20_LOG(LogLevel::INSTANCE.isInfo(),"Connected: "<<peerRemote<<", fd: "<<iSocket);

				return pConnectionFactory->createFileHandle(iSocket, peerRemote);
			}

		}

    IA20_LOG(LogLevel::INSTANCE.isInfo(),"Error in connect: "<<errno<<" "<<strerror(errno));
	}

	IA20_THROW(SystemException("Couldn't connect any address from: ")<<peerLocal<<", to: "<<peerRemote);
}
/*************************************************************************/
void Client::setLocalPeer(const Peer& peerLocal){
	IA20_TRACER;
	this->peerLocal=peerLocal;
}
/*************************************************************************/
}
}
}
}