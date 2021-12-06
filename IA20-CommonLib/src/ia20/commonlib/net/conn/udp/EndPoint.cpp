/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/EndPoint.cpp
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

#include "EndPoint.h"
#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace Net {
namespace Conn {
namespace UDP {

/*************************************************************************/
EndPoint::EndPoint(const String& strGroup, const String& strInterface, unsigned int iPort,
                   PacketSizeType iMaxPacketSize):
	strInterface(strInterface),
	strGroup(strGroup),
	iPort(iPort),
  iMaxPacketSize(iMaxPacketSize){
	IA20_TRACER;
}
/*************************************************************************/
EndPoint::EndPoint(const EndPoint& other):
	strInterface(other.strInterface),
	strGroup(other.strGroup),
	iPort(other.iPort),
  iMaxPacketSize(other.iMaxPacketSize){
	IA20_TRACER;
}
/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const EndPoint& ep){
	os<<"EP:["<<ep.strInterface<<","<<ep.strGroup<<","<<ep.iPort<<","<<ep.iMaxPacketSize<<"]";
	return os;
}
/*************************************************************************/

}
}
}
}