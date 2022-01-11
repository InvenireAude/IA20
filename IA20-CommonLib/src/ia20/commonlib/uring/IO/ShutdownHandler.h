/*
 * File: ShutdownHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_ShutdownHandler_H_
#define _IA20_URing_IO_ShutdownHandler_H_

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
/** The ShutdownHandler class.
 *
 */
class ShutdownHandler : public EventHandler, public FileHandler{
public:

	virtual ~ShutdownHandler() throw();

  void prepare(int how);
  virtual void handle(int iResult);

	ShutdownHandler(RingHandler* pRingHandler, IA20::Net::Conn::TCP::FileHandle* pFileHandle);

protected:


  virtual void handleShutdown(int iResult)  = 0;

};
/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_ShutdownHandler_H_ */

