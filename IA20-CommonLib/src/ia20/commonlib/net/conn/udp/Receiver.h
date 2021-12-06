/*
 * File: IA20-CommonLib/src/commonlib/net/mcast/Receiver.h
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


#ifndef _IA20_Net_Conn_UDP_Receiver_H_
#define _IA20_Net_Conn_UDP_Receiver_H_

#include <ia20/commonlib/types.h>
#include "Base.h"
#include "../Address.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace UDP {

/*************************************************************************/
/** The Receiver class.
 *
 */
class Receiver : public virtual Base {
public:

	Receiver(unsigned int iPort);
	virtual ~Receiver() throw();

	void bind();

	bool receive(void *pData, size_t iBufferLen, Address& address);

protected:


};
/*************************************************************************/
}
}
}
}

#endif /* _IA20_Net_Conn_UDP_Receiver_H_ */
