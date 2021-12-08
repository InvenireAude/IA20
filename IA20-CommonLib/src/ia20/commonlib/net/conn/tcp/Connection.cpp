/*
 * File: IA20-CommonLib/src/commonlib/net/Connection.cpp
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
#include "Connection.h"

#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/exception/SystemException.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "ConnectionFactory.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
Connection::Connection(ConnectionFactory *pConnectionFactory):
 bNoDelay(false),
 pConnectionFactory(pConnectionFactory){
	IA20_TRACER;
}
/*************************************************************************/
Connection::Connection(const Peer& peerLocal, ConnectionFactory *pConnectionFactory):
  peerLocal(peerLocal),
  bNoDelay(false),
  pConnectionFactory(pConnectionFactory){
	IA20_TRACER;
}
/*************************************************************************/
Connection::~Connection() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Connection::setNoDelay(bool bValue){
	this->bNoDelay = bValue;
}
/*************************************************************************/
void Connection::setNoDelayImpl(int iSocket, bool bValue)const{
	IA20_TRACER;

	int optval = bValue  ? 1 : 0;
	IA20_LOG(LogLevel::INSTANCE.isInfo(), "setNoDelay("<<optval<<")");

	int rc = setsockopt(iSocket, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);

	if(rc < 0)
		IA20_THROW(SystemException("setNoDelay(...) failed."));

}
/*************************************************************************/
}
}
}
}