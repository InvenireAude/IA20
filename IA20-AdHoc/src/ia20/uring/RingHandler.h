/*
 * File: RingHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_RingHandler_H_
#define _IA20_URing_RingHandler_H_

#include <ia20/commonlib/commonlib.h>

#include <liburing.h>

#include <ia20/commonlib/sys/FileDescriptorBase.h>
#include <ia20/commonlib/net/conn/Address.h>

#include "EventHandler.h"

namespace IA20 {
namespace URing {

class BufferSet;

/*************************************************************************/
/** The RingHandler class.
 *
 */
class RingHandler {
public:

	virtual ~RingHandler() throw();

	RingHandler();

  void handle();

  void prepareAccept(EventHandler* pEventHandler, int fd, Net::Conn::Address& address, int flags);
  void prepareRead(EventHandler* pEventHandler, int fd, struct iovec* iovec, off_t iOffset);
  void prepareWrite(EventHandler* pEventHandler, int fd, struct iovec* iovec, off_t iOffset);

protected:

  static const int CDefaultSize = 32;

  struct io_uring ring;

  void onEvent(struct io_uring_cqe *cqe);

  friend class ::IA20::URing::BufferSet;
};
/*************************************************************************/
}
}

#endif /* _IA20_URing_RingHandler_H_ */
