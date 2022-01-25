/*
 * File: ReadHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "ReadHandler.h"

#include <ia20/commonlib/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
ReadHandler::ReadHandler(RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
  EventHandler(pRingHandler),
  FileHandler(pFileHandle),
  iOffset(0){
	IA20_TRACER;

}
/*************************************************************************/
ReadHandler::~ReadHandler() throw(){
	IA20_TRACER;

}
/*************************************************************************/
void ReadHandler::prepare(){
	IA20_TRACER;
   pRingHandler->prepareRead(this, pFileHandle->iFileDescriptor, &iovec, 0);
   bInProgress = true;
}
/*************************************************************************/
void ReadHandler::handle(int iResult){
	IA20_TRACER;
  
  bInProgress = false;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0 && iResult != -11)
    IA20_THROW(URingException("Failure in ReadHandler", -iResult));

  handleRead(iResult);
}
/*************************************************************************/
}
}
}
