/*
 * File: Context.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Context.h"


namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
Context::Context(Memory::SharableMemoryPool* pMemoryPool):
	ptrTask(new (pMemoryPool->allocate<Listener::Task>(NULL))
              Listener::Task(Listener::Task::CA_SetContent), pMemoryPool->getDeleter()),
	sbl(pMemoryPool, ptrTask.get()),
	writer(sbl),
	iExpectingLength(0){
	IA20_TRACER;
	
	ptrTask->setMessage(sbl.getHead());
}
/*************************************************************************/
// Context::~Context() throw(){
// 	IA20_TRACER;
// }
/*************************************************************************/
}
}
}
