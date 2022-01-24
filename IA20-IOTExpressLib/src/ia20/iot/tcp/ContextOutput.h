/*
 * File: ContextOutput.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_ContextOutput_H_
#define _IA20_IOT_TCP_ContextOutput_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/memory/SharableMemoryPool.h>

#include <ia20/iot/Listener.h>

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The ContextOutput class.
 *
 */
class ContextOutput {
public:

	virtual ~ContextOutput() throw();
	ContextOutput();

	virtual bool next() = 0;
	virtual uint8_t  *getData() = 0;
	virtual Memory::StreamBufferList::DataLengthType getDataLength()const = 0;
	
protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_ContextOutput_H_ */
