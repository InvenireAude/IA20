/*
 * File: Acceptor.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_Acceptor_H_
#define _IA20_IOT_TCP_Acceptor_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>

#include "Server.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The Acceptor class.
 *
 */
class Listener;
class Acceptor : public URing::IO::TCP::AsyncServer::Acceptor {
  public:

    Acceptor(URing::IO::TCP::AsyncServer* pAsyncServer, 
			Listener *pListener);

	~Acceptor();

  protected:

    virtual void handleImpl(Net::Conn::TCP::FileHandle* pFileHandle);
 
  	Listener *pListener;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_Acceptor_H_ */
