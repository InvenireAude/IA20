/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/EndPoint.h
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

#ifndef _IA20_Net_Bus_EndPoint_H_
#define _IA20_Net_Bus_EndPoint_H_

#include <ia20/commonlib/types.h>

namespace IA20 {
namespace Net {
namespace Conn {
namespace UDP {

/*************************************************************************/
/** The EndPoint class.
 *
 */
class EndPoint {
public:

  typedef unsigned int PacketSizeType;


	inline const String&  getInterface()const{
		return strInterface;
	}

	inline const String&  getGroup()const{
		return strGroup;
	}

	inline unsigned int getPort()const{
		return iPort;
	}

	inline PacketSizeType getMaxPacketSize()const{
		return iMaxPacketSize;
	}

	inline EndPoint& operator=(const EndPoint& other){
        this->strInterface=other.strInterface;
        this->strGroup=other.strGroup;
        this->iPort=other.iPort;
        this->iMaxPacketSize=other.iMaxPacketSize;
		return *this;
	}


	EndPoint(const String& strGroup, const String& strInterface, unsigned int iPort, PacketSizeType iMaxPacketSize);
	EndPoint(const EndPoint& other);

	friend std::ostream& operator<<(std::ostream&, const EndPoint&);

protected:
	String   strInterface;
	String   strGroup;
	unsigned int iPort;
  PacketSizeType iMaxPacketSize;
};
/*************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Bus_EndPoint_H_ */
