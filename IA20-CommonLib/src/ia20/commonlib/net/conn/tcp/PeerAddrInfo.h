/*
 * File: IA20-CommonLib/src/commonlib/net/PeerAddrInfo.h
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
#ifndef _IA20_Net_Conn_TCP_PeerAddrInfo_H_
#define _IA20_Net_Conn_TCP_PeerAddrInfo_H_

#include "Peer.h"

struct addrinfo;

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
/** The PeerAddrInfo class.
 *
 */
class PeerAddrInfo {
public:

	~PeerAddrInfo();
	PeerAddrInfo(const Peer& peer, bool bPassive=false);

	class const_iterator {
	public:
		const_iterator(const const_iterator& o) {
			this->pAddrInfo = o.pAddrInfo;
		}

		const_iterator& operator=(const const_iterator& o) {
			this->pAddrInfo = o.pAddrInfo;
			return *this;
		}

		const_iterator& operator++(){
			if(pAddrInfo)
				pAddrInfo=pAddrInfo->ai_next;
			return *this;
		}

		const struct addrinfo * operator*(){ return pAddrInfo;};

		bool operator==(const const_iterator& o){
			return pAddrInfo == o.pAddrInfo;
		}

		bool operator!=(const const_iterator& o){
			return pAddrInfo != o.pAddrInfo;
		}

	protected:
		const_iterator(const addrinfo *pAddrInfo):pAddrInfo(pAddrInfo){};
		const struct addrinfo *pAddrInfo;
		friend class PeerAddrInfo;
	};

	const_iterator begin(){
		return const_iterator(pAddrInfo);
	}

	const_iterator end(){
		return const_iterator(0);
	}


protected:

	const Peer& peer;
	addrinfo *pAddrInfo;

};

/*************************************************************************/
}
}
}
}
#endif /* _IA20_Net_Conn_TCP_PeerAddrInfo_H_ */
