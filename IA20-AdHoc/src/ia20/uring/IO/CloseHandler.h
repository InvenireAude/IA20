/*
 * File: CloseHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_CloseHandler_H_
#define _IA20_URing_IO_CloseHandler_H_

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
/** The CloseHandler class.
 *
 */
class CloseHandler : public EventHandler, public FileHandler{
public:

	virtual ~CloseHandler() throw();

  void prepare();
  virtual void handle(int iResult);

	CloseHandler(RingHandler* pRingHandler, IA20::Net::Conn::TCP::FileHandle* pFileHandle);

protected:


  virtual void handleClose(int iResult)  = 0;

};
/*************************************************************************/
}
}
}

#endif /* _IA20_URing_IO_CloseHandler_H_ */

