/*
 * File: IA20-CommonLib/src/commonlib/net/Peer.h
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
#ifndef _IA20_Net_Conn_TCP_Peer_H_
#define _IA20_Net_Conn_TCP_Peer_H_

#include <ia20/commonlib/types.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
class Peer{
public:

	Peer();
	Peer(const Peer& other);
	Peer(int iPort);
	Peer(const String& strAddress, int iPort=-1);
	Peer(const String& strAddress, const String& strService);

	~Peer() throw();

	inline bool isDefault()const{ return strAddress.empty() && strService.empty(); };
	inline bool hasAddress()const{ return !strAddress.empty(); };
	inline bool hasService()const{ return !strService.empty(); };

	Peer& operator=(const Peer& other);

	const String& getAddress()const { return strAddress; }
	const String& getService()const { return strService; };

	operator String()const{
		StringStream ssTmp;
		ssTmp<<(*this);
		return ssTmp.str();
	}

	friend std::ostream& operator<<(std::ostream&, const Peer& peer);

protected:

	String strAddress;
	String strService;

};
/*************************************************************************/
std::ostream& operator<<(std::ostream&, const Peer& peer);
/*************************************************************************/
}
}
}
}
#endif /* _IA20_Net_Conn_TCP_Peer_H_ */
