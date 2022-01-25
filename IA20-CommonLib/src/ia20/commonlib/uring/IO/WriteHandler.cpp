/*
 * File: WriteHandler.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "WriteHandler.h"

#include <ia20/commonlib/uring/URingException.h>

namespace IA20 {
namespace URing {
namespace IO {

/*************************************************************************/
WriteHandler::WriteHandler(RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
  EventHandler(pRingHandler),
  FileHandler(pFileHandle),
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
void WriteHandler::prepare(){
	IA20_TRACER;
   iovecBackup = iovec;
   pRingHandler->prepareWrite(this, pFileHandle->iFileDescriptor, &iovec, 0);
   bInProgress = true;
}
/*************************************************************************/
void WriteHandler::handle(int iResult){
	IA20_TRACER;
  bInProgress = false;
  IA20_LOG(LogLevel::INSTANCE.isSystem(), "handle: res="<<iResult);

  if(iResult < 0)
    IA20_THROW(URingException("Failure in WriteHandler", -iResult));

  if(iovec.iov_len > iResult){
   // IA20_LOG(true, "left to write ="<<iovec.iov_len<<" - "<<iResult);
    iovec.iov_base = (uint8_t*)iovec.iov_base + iResult;
    iovec.iov_len -= iResult;
    pRingHandler->prepareWrite(this, pFileHandle->iFileDescriptor, &iovec, 0);
  }else{
    handleWrite(iResult);
  }

}
/*************************************************************************/
}
}
}
