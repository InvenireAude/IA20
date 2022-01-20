/*
 * File: Context.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_Context_H_
#define _IA20_IOT_TCP_Context_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/memory/SharableMemoryPool.h>

#include "../Listener.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The Context class.
 *
 */
class Context {

	public:
 
 		Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> ptrTask;

  		uint32_t iExpectingLength;
		
		Memory::StreamBufferList sbl;
		Memory::StreamBufferList::Writer writer;

		Context(Memory::SharableMemoryPool* pMemoryPool);

		~Context(){}
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_Context_H_ */
