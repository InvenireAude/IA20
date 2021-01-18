/*
 * File: ReadHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "ReadHandler.h"

#include <ia20/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
ReadHandler::ReadHandler(Net::Conn::TCP::FileHandle* pFileHandle):
  pFileHandle(pFileHandle),
  iDataLen(0),
  iOffset(0){
	IA20_TRACER;

  iovec.iov_base = malloc(4096);
  iovec.iov_len  = 4096;
}
/*************************************************************************/
ReadHandler::~ReadHandler() throw(){
	IA20_TRACER;
  free(iovec.iov_base);
}
/*************************************************************************/
void ReadHandler::prepare(RingHandler* pRingHandler){
	IA20_TRACER;
   pRingHandler->prepareRead(this, pFileHandle->iFileDescriptor, &iovec, 0);
}
/*************************************************************************/
void ReadHandler::handle(int iResult){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0)
    IA20_THROW(URingException("Failure in ReadHandler", -iResult));

  iDataLen = iResult;
  handleImpl();
}
/*************************************************************************/
}
}
}
