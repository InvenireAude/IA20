/*
 * File: WriteHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_WriteHandler_H_
#define _IA20_URing_IO_WriteHandler_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/commonlib/uring/EventHandler.h>
#include <ia20/commonlib/uring/RingHandler.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <liburing.h>

#include<functional>
#include<tuple>

#include "FileHandler.h"

namespace IA20 {
namespace URing {

namespace IO {

/*************************************************************************/
/** The WriteHandler class.
 *
 */
class WriteHandler : public EventHandler, public FileHandler{
public:

	virtual ~WriteHandler() throw();

  void prepare();
  virtual void handle(int iResult);


protected:

	WriteHandler(RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle);

  struct iovec iovec;
  struct iovec iovecBackup;
  
  off_t        iOffset;

  
  virtual void handleWrite(off_t  iDataLen) = 0;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_WriteHandler_H_ */

