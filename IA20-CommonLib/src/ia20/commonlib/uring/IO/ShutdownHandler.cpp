/*
 * File: ShutdownHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "ShutdownHandler.h"

#include <ia20/commonlib/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
ShutdownHandler::ShutdownHandler(RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
  EventHandler(pRingHandler),
  FileHandler(pFileHandle){
	IA20_TRACER;
}
/*************************************************************************/
ShutdownHandler::~ShutdownHandler() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void ShutdownHandler::prepare(int how){
	IA20_TRACER;
   pRingHandler->prepareShutdown(this, pFileHandle->iFileDescriptor, how);
}
/*************************************************************************/
void ShutdownHandler::handle(int iResult){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0)
    IA20_THROW(URingException("Failure in ShutdownHandler", -iResult));

  handleShutdown(iResult);
}
/*************************************************************************/
}
}
}
