/*
 * File: ReadHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_ReadHandler_H_
#define _IA20_URing_IO_ReadHandler_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/uring/EventHandler.h>
#include <ia20/uring/RingHandler.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <liburing.h>

#include<functional>
#include<tuple>

#include "FileHandler.h"


namespace IA20 {
namespace URing {

namespace IO {

/*************************************************************************/
/** The ReadHandler class.
 *
 */
class ReadHandler : public EventHandler, public FileHandler{
public:

	virtual ~ReadHandler() throw();

  void prepare();
  virtual void handle(int iResult);

	ReadHandler(RingHandler* pRingHandler, IA20::Net::Conn::TCP::FileHandle* pFileHandle);

protected:

  struct iovec iovec;
  off_t        iOffset;

  virtual void handleRead(off_t iDataLen)  = 0;

};
/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_ReadHandler_H_ */

