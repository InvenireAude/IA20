/*
 * File: IA20-CommonLib/src/commonlib/net/FileHandle.h
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
#ifndef _IA20_Net_Conn_TCP_FileHandle_H_
#define _IA20_Net_Conn_TCP_FileHandle_H_


#include <ia20/commonlib/sys/FileDescriptorBase.h>

#include "Peer.h"

namespace IA20 {
namespace Net {
namespace Conn {
namespace TCP {

/*************************************************************************/
/** The FileHandle class.
 *
 */

class FileHandle : public SYS::FileDescriptorBase {
public:

	virtual ~FileHandle()throw();

	virtual bool read(void *pData, size_t iBufferLen, size_t& iDataSize);
	virtual bool write(const void *pData, size_t iDataSize, size_t& iWritten);

	void setNonBlocking();

	void setTimeout(int iTimeout);
	static const int  C_UnLimited = -1;

	const Peer& getPeer()const;

	FileHandle(int iFileDescriptor, const Peer& peer);

	void close();

	enum WaitMode{
		WM_Read,
		WM_Write
	};

	bool waitForData(WaitMode iMode);


	int iTimeout;

	Peer peer;

	static const int  C_NonBlock = -2;

};
/*************************************************************************/
}
}
}
}
#endif /* _IA20_Net_Conn_TCP_FileHandle_H_ */
