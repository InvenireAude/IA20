/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/Sender.h
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


#ifndef _IA20_Net_Conn_UDP_Sender_H_
#define _IA20_Net_Conn_UDP_Sender_H_

#include <ia20/commonlib/types.h>
#include "Base.h"

#include "../Address.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace UDP {

/*************************************************************************/
/** The Sender class.
 *
 */
class Sender : public virtual Base {
public:

	Sender(unsigned int iPort);
	virtual ~Sender() throw();

	void send(const Address& address, const void *pData, size_t iDataSize);
	void send(const Address& address, const void *pData, size_t iDataSize, size_t& iWritten);

};
/*************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Conn_UDP_Sender_H_ */
