/*
 * File: IA20-CommonLib/src/commonlib/net/Server.cpp
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
#include "Server.h"

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
#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/threads/Thread.h>

#include "PeerAddrInfo.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
Server::Server(const Peer& peerLocal, ConnectionFactory *pConnectionFactory):
  ServerBase(peerLocal, pConnectionFactory){
	IA20_TRACER;
}
/*************************************************************************/
Server::~Server() throw(){
	IA20_TRACER;
	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"closing server: "<<peerLocal);
}
/*************************************************************************/
FileHandle* Server::accept()const{
	IA20_TRACER;

	int iFileDescriptor = -1;

	struct sockaddr_in sinRemote;
	socklen_t          socklen=sizeof(sinRemote);
	sinRemote.sin_family = AF_INET;
	{
		Thread::Cancellation ca(true);
		iFileDescriptor = ::accept(this->iFileDescriptor, (struct sockaddr*)&sinRemote, &socklen);
	}

  return acceptImpl(iFileDescriptor, &sinRemote, socklen);
}
/*************************************************************************/
}
}
}
}