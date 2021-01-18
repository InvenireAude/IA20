/*
 * File: WriteHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_WriteHandler_H_
#define _IA20_URing_IO_WriteHandler_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/uring/EventHandler.h>
#include <ia20/uring/RingHandler.h>
#include <ia20/net/conn/tcp/FileHandle.h>
#include <liburing.h>

#include<functional>
#include<tuple>

namespace IA20 {
namespace URing {

namespace IO {

/*************************************************************************/
/** The WriteHandler class.
 *
 */
class WriteHandler : public EventHandler{
public:

	virtual ~WriteHandler() throw();

  void prepare(RingHandler* pRingHandler);
  virtual void handle(int iResult);


protected:

	WriteHandler(Net::Conn::TCP::FileHandle* pFileHandle);

  struct iovec iovec;
  off_t        iOffset;
  off_t        iDataLen;

  virtual void handleImpl() = 0;

  Net::Conn::TCP::FileHandle* pFileHandle;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_WriteHandler_H_ */

