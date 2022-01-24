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
 
		Context(Memory::SharableMemoryPool* pMemoryPool);
		~Context(){}

		Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> finish(uint64_t iReferenceId, 
					Connection::HandleType aConnectionHandle);


		inline uint32_t getExpectedLength()const{
			return data.iExpectedLength;
		}

		inline void setExpectedLength(uint32_t iExpectedLength){
			data.iExpectedLength = iExpectedLength;
		}
		
		inline void appendExpected(const uint8_t* pData){
			IA20_LOG(true, "Appending expected: "<<(int)data.iExpectedLength);
			data.writer.write(pData, data.iExpectedLength);
			data.iExpectedLength = 0;
		}

		inline void append(const uint8_t* pData, uint32_t iDataLength){
			data.writer.write(pData, iDataLength);
			data.iExpectedLength -= iDataLength;
		}

	protected:
	
	struct Data {
		Data(Memory::SharableMemoryPool* pMemoryPool);
 		Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> ptrTask;
  		uint32_t iExpectedLength;
		Memory::StreamBufferList sbl;
		Memory::StreamBufferList::Writer writer;
	};

	Data data;

	Memory::SharableMemoryPool* pMemoryPool;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_Context_H_ */
