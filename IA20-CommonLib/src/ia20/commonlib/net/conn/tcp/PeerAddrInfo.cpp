/*
 * File: IA20-CommonLib/src/commonlib/net/PeerAddrInfo.cpp
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
#include "PeerAddrInfo.h"
#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/exception/SystemException.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
PeerAddrInfo::PeerAddrInfo(const Peer& peer,  bool bPassive):
		pAddrInfo(0),
		peer(peer){
	IA20_TRACER;

	const char* sAddress="0.0.0.0";
	const char* sService=0;

	if(peer.hasAddress())
		sAddress=peer.getAddress().c_str();

	if(peer.hasService())
		sService=peer.getService().c_str();

	struct addrinfo hints;

	hints.ai_family = AF_INET; 		 /* AF_UNSPEC for IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;

   	hints.ai_flags = /*AI_ADDRCONFIG | */ ( bPassive ? AI_PASSIVE : 0 );

    hints.ai_protocol  = IPPROTO_TCP;           /* Any protocol */
    hints.ai_addrlen = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int iRes=::getaddrinfo(sAddress, sService, &hints, &pAddrInfo);
    if(iRes) {
        IA20_THROW(SystemException()<<" "<<peer<<" : "<<gai_strerror(iRes))
    }

}
/*************************************************************************/
PeerAddrInfo::~PeerAddrInfo(){
	IA20_TRACER;
	if(pAddrInfo)
		freeaddrinfo(pAddrInfo);
}
/*************************************************************************/
}
}
}
}