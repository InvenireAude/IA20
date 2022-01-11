/*
 * File: CloseHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "CloseHandler.h"

#include <ia20/commonlib/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
CloseHandler::CloseHandler(RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
  EventHandler(pRingHandler),
  FileHandler(pFileHandle){
	IA20_TRACER;
}
/*************************************************************************/
CloseHandler::~CloseHandler() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void CloseHandler::prepare(){
	IA20_TRACER;
   pRingHandler->prepareClose(this, pFileHandle->iFileDescriptor);
}
/*************************************************************************/
void CloseHandler::handle(int iResult){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0)
    IA20_THROW(URingException("Failure in CloseHandler", -iResult));

  handleClose(iResult);
}
/*************************************************************************/
}
}
}
