/*
 * File: IA20-CommonLib/src/commonlib/net/ServerBase.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
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

#include <ia20/commonlib/commonlib.h>

#include "ServerBase.h"
#include "PeerAddrInfo.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
ServerBase::ServerBase(const Peer& peerLocal, ConnectionFactory *pConnectionFactory):
  Connection(peerLocal, pConnectionFactory){
	IA20_TRACER;
	bind();
}
/*************************************************************************/
ServerBase::~ServerBase() throw(){
	IA20_TRACER;
	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"closing server: "<<peerLocal);
}
/*************************************************************************/
void ServerBase::bind(){
	IA20_TRACER;

	PeerAddrInfo ai(peerLocal,true);

	for(PeerAddrInfo::const_iterator it=ai.begin(); it != ai.end(); ++it){

		if((iFileDescriptor = socket((*it)->ai_family, (*it)->ai_socktype, (*it)->ai_protocol)) < 0)
			IA20_THROW(SystemException("socket(): ")<<peerLocal);


		int optval = 1;
		setsockopt(iFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

		IA20_LOG(LogLevel::INSTANCE.isInfo(),"ServerBase: "<<iFileDescriptor<<", addr="
					<<inet_ntoa(((struct sockaddr_in*)((*it)->ai_addr))->sin_addr)
					<<", port="<<(ntohs(((struct sockaddr_in*)((*it)->ai_addr))->sin_port))
					<<", family="<<((struct sockaddr_in*)((*it)->ai_addr))->sin_family);

		if(::bind(iFileDescriptor, (*it)->ai_addr, (*it)->ai_addrlen) == 0){

			if(listen(iFileDescriptor, 128)  == -1)
				IA20_THROW(SystemException("listen: ")<<peerLocal);

			IA20_LOG(LogLevel::INSTANCE.isInfo(),"ServerBase started: "<<peerLocal<<", fd="<<(int)iFileDescriptor);

			return;
		}

	}

	IA20_THROW(SystemException("Couldn't bind on any address for: ")<<peerLocal);

}
/*************************************************************************/
FileHandle* ServerBase::acceptImpl(int iFileDescriptor, struct sockaddr_in* pRemote, socklen_t socklen)const{
	IA20_TRACER;

	if(iFileDescriptor < 0)
		IA20_THROW(SystemException("accept:"));

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"accepted:"<<peerLocal<<", fd="<<iFileDescriptor);

	char sTmp[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(pRemote->sin_addr), sTmp, INET_ADDRSTRLEN);

	if(bNoDelay)
	  setNoDelayImpl(iFileDescriptor, 1);

  Peer peer(sTmp, ntohl(pRemote->sin_port)); //TODO refactor Peer and Address classes ...

  return pConnectionFactory->createFileHandle(iFileDescriptor, peer);
}
/*************************************************************************/
}
}
}
}