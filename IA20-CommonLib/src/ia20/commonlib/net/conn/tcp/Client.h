/*
 * File: IA20-CommonLib/src/commonlib/net/Client.h
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
#ifndef _IA20_Net_Conn_TCP_Client_H_
#define _IA20_Net_Conn_TCP_Client_H_

#include "Peer.h"
#include "FileHandle.h"
#include "Connection.h"
#include "ConnectionFactory.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
class Client : public Connection {
public:

	Client(const Peer& peerRemote, ConnectionFactory *pConnectionFactory);
	Client(const Peer& peerRemote, const Peer& peerLocal, ConnectionFactory *pConnectionFactory);

	~Client() throw();

	FileHandle* connect()const;

	void setLocalPeer(const Peer& peerLocal);

	const Peer& getRemote()const{
		return peerRemote;
	}

private:
	Peer peerRemote;
};
/*************************************************************************/
}
}
}
}
#endif /* _IA20_Net_Conn_TCP_Client_H_ */
