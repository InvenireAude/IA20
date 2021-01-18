/*
 * File: WriteHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "WriteHandler.h"

#include <ia20/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
WriteHandler::WriteHandler(Net::Conn::TCP::FileHandle* pFileHandle):
  pFileHandle(pFileHandle),
  iDataLen(0),
  iOffset(0){
	IA20_TRACER;

  iovec.iov_base = 0;
  iovec.iov_len = 0;
}
/*************************************************************************/
WriteHandler::~WriteHandler() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void WriteHandler::prepare(RingHandler* pRingHandler){
	IA20_TRACER;
   pRingHandler->prepareWrite(this, pFileHandle->iFileDescriptor, &iovec, 0);
}
/*************************************************************************/
void WriteHandler::handle(int iResult){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0)
    IA20_THROW(URingException("Failure in WriteHandler", -iResult));

  iDataLen = iResult;
  handleImpl();
}
/*************************************************************************/
}
}
}
