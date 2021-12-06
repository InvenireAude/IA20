/*
 * File: IA20-CommonLib/src/commonlib/net/ServerBase.h
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
#ifndef _IA20_Net_Conn_TCP_ServerBase_H_
#define _IA20_Net_Conn_TCP_ServerBase_H_

#include <ia20/commonlib/sys/FileDescriptorBase.h>

#include "FileHandle.h"
#include "Peer.h"
#include "Connection.h"

#include "ConnectionFactory.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
/** The ServerBase class.
 *
 */
class ServerBase : public Connection, public SYS::FileDescriptorBase {
public:

	ServerBase(const Peer& peerLocal, ConnectionFactory *pConnectionFactory);

	void bind();

	~ServerBase() throw();

protected:

	FileHandle* acceptImpl(int iFileDescriptor, struct sockaddr_in* pRemote, socklen_t socklen)const;

};

/*************************************************************************/
}
}
}
}
#endif /* _IA20_Net_Conn_TCP_ServerBase_H_ */
