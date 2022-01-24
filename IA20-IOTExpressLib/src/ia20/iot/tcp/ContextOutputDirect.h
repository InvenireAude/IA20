/*
 * File: ContextOutputDirect.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_ContextOutputDirect_H_
#define _IA20_IOT_TCP_ContextOutputDirect_H_

#include <ia20/commonlib/commonlib.h>

#include "ContextOutput.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The ContextOutputDirect class.
 *
 */
class ContextOutputDirect : public ContextOutput {
public:

	virtual ~ContextOutputDirect() throw();
	ContextOutputDirect(uint8_t *pMessage);

	virtual bool next();
	virtual uint8_t  *getData();
	virtual Memory::StreamBufferList::DataLengthType getDataLength()const;

protected:

 	Memory::StreamBufferList sbl;
    Memory::StreamBufferList::Reader reader;
 
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_ContextOutputDirect_H_ */
