/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/Sender.cpp
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

#include "Sender.h"

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
namespace UDP {

/*************************************************************************/
Sender::Sender(unsigned int iPort):
	Base(iPort){
	IA20_TRACER;
}
/*************************************************************************/
Sender::~Sender() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Sender::send(const Address& address, const void *pData, size_t iDataSize, size_t& iWritten){
	IA20_TRACER;

	ssize_t iResult = 0;

	if(LogLevel::INSTANCE.isData()){
		String strData;
		for(int i=0;i<iDataSize;i++)
			strData += String("") + (*(((char*)pData)+i) >= ' ' ? *(((char*)pData)+i) : '.');
		IA20_LOG(LogLevel::INSTANCE.isData(),"WRITE:"<<iDataSize<<" ["<<strData<<"]");
	}

	{
		Thread::Cancellation ca(true);
		iResult = ::sendto(iSocket, pData, iDataSize, 0, (struct sockaddr *)&address.address, sizeof(address.address));
	}

	if(iResult < 0){
		IA20_LOG(LogLevel::INSTANCE.isError(),"SEND:"<<(void*)pData<<"len="<<iDataSize);
		IA20_THROW(SystemException("write",errno));
	}

	iWritten = iResult;
}
/*************************************************************************/
void Sender::send(const Address& address, const void *pData, size_t iDataSize){
	IA20_TRACER;
	size_t iWritten;
	Sender::send(address, pData, iDataSize, iWritten);

	if(iWritten != iDataSize){
		IA20_THROW(SystemException("Sender::send(), iWritten != iDataSize"));
	}
}
/*************************************************************************/
}
}
}
}