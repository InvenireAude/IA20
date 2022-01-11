/*
 * File: AsyncServer.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_IO_TCP_AsyncServer_H_
#define _IA20_URing_IO_TCP_AsyncServer_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/commonlib/net/conn/tcp/ServerBase.h>
#include <ia20/commonlib/net/conn/Address.h>

#include <ia20/commonlib/uring/EventHandler.h>

#include <ia20/commonlib/net/conn/tcp/FileHandle.h>

#include<functional>
#include<tuple>

namespace IA20 {
namespace URing {

class RingHandler;
namespace IO {
namespace TCP {

/*************************************************************************/
/** The AsyncServer class.
 *
 */
class AsyncServer : public Net::Conn::TCP::ServerBase {
public:

	virtual ~AsyncServer() throw();

  class Acceptor : public EventHandler {
    public:


      Acceptor(RingHandler* pRingHandler, AsyncServer* pAsyncServer);

      void prepare();
      virtual void handle(int iResult);

    protected:

      virtual void handleImpl(Net::Conn::TCP::FileHandle* pFileHandle) =0;

      AsyncServer*       pAsyncServer;
      Net::Conn::Address address;
  };

	AsyncServer(const Net::Conn::TCP::Peer& peerLocal, Net::Conn::TCP::ConnectionFactory *pConnectionFactory);
protected:

};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_URing_IO_TCP_AsyncServer_H_ */
