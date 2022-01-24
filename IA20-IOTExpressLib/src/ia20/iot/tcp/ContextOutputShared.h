/*
 * File: ContextOutputShared.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_ContextOutputShared_H_
#define _IA20_IOT_TCP_ContextOutputShared_H_

#include <ia20/commonlib/commonlib.h>

#include "ContextOutput.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The ContextOutputShared class.
 *
 */
class ContextOutputShared : public ContextOutput {
public:

	virtual ~ContextOutputShared() throw();
	ContextOutputShared(uint8_t *pMessage, uint8_t* pPayLoad);

	virtual bool next();
	virtual uint8_t  *getData();
	virtual Memory::StreamBufferList::DataLengthType getDataLength()const;

protected:

 	Memory::StreamBufferList sbl1;
	Memory::StreamBufferList sbl2;
    Memory::StreamBufferList::Reader reader1;
 	Memory::StreamBufferList::Reader reader2;

	static const uint32_t CBufferSize = 1500;
	uint8_t buffer[CBufferSize];

	uint8_t iReaderIdx;
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_ContextOutputShared_H_ */
