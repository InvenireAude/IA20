/*
 * File: IA20-CommonLib/src/commonlib/net/Peer.cpp
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

#include <ia20/commonlib/commonlib.h>

#include "Peer.h"

namespace IA20 {
namespace Net {
namespace Conn {

/*************************************************************************/
Peer::Peer(){}
/*************************************************************************/
Peer::Peer(const Peer& other){
	this->strAddress=other.strAddress;
	this->strService=other.strService;
}
/*************************************************************************/
Peer::Peer(const String& strAddress, const String& strService):
		strAddress(strAddress),strService(strService){}
/*************************************************************************/
Peer::Peer(int iPort):
	strService(TypeTools::IntToString(iPort)){
	IA20_TRACER;
}
/*************************************************************************/
Peer::Peer(const String& strAddress, int iPort):
	strAddress(strAddress),strService(iPort > 0 ? TypeTools::IntToString(iPort) : ""){
	IA20_TRACER;
}
/*************************************************************************/
Peer::~Peer() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Peer& Peer::operator=(const Peer& other){
	IA20_TRACER;

	this->strAddress=other.strAddress;
	this->strService=other.strService;

	return *this;
}
/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const Peer& peer){

	os<<(!peer.strAddress.empty() ? peer.strAddress : "0.0.0.0");

	if(!peer.strService.empty())
		os<<":"<<peer.strService;

	return os;
}
/*************************************************************************/
}
}
}