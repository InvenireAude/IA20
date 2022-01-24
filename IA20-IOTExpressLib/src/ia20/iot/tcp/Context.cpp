/*
 * File: Context.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Context.h"

#include <ia20/iot/Connection.h>

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
Context::Context(Memory::SharableMemoryPool* pMemoryPool):
	pMemoryPool(pMemoryPool),
	data(pMemoryPool){};
/*************************************************************************/
Context::Data::Data(Memory::SharableMemoryPool* pMemoryPool):
	ptrTask(new (pMemoryPool->allocate<Listener::Task>(NULL))
                Listener::Task(Listener::Task::CA_InputMsg), pMemoryPool->getDeleter()),
	sbl(pMemoryPool, ptrTask.get()),
	writer(sbl),
	iExpectedLength(0){	
	ptrTask->setMessage(sbl.getHead());
}
/*************************************************************************/
Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> Context::finish(
					uint64_t iReferenceId, 
					Connection::HandleType aConnectionHandle){
	IA20_TRACER;


	data.ptrTask->setReferenceId(iReferenceId);
	data.ptrTask->setConnectionHandle(aConnectionHandle);

	Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> ptrResult(std::move(data.ptrTask));

	data.~Data();
	new (&data)Data(pMemoryPool);

	return std::move(ptrResult);
}
/*************************************************************************/
// Context::~Context() throw(){
// 	IA20_TRACER;
// }
/*************************************************************************/
}
}
}
