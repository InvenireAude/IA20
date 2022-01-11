/*
 * File: AsyncServer.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "AsyncServer.h"

#include <ia20/commonlib/uring/RingHandler.h>

namespace IA20 {
namespace URing {
namespace IO {
namespace TCP {

/*************************************************************************/
AsyncServer::Acceptor::Acceptor(RingHandler* pRingHandler, AsyncServer* pAsyncServer):
  EventHandler(pRingHandler),
  pAsyncServer(pAsyncServer){
	IA20_TRACER;

	address.address.ss_family = AF_INET;
}
/*************************************************************************/
void AsyncServer::Acceptor::handle(int iResult){
	IA20_TRACER;

  Net::Conn::TCP::FileHandle *pFileHandle = pAsyncServer->acceptImpl(iResult,
               (struct sockaddr_in *)&address.address, address.iAddressLen);

  handleImpl(pFileHandle);
}
/*************************************************************************/
void AsyncServer::Acceptor::prepare(){
  IA20_TRACER;

  pRingHandler->prepareAccept(this, pAsyncServer->iFileDescriptor, address, 0);
}
/*************************************************************************/
AsyncServer::AsyncServer(const Net::Conn::TCP::Peer& peerLocal, Net::Conn::TCP::ConnectionFactory *pConnectionFactory):
 ServerBase(peerLocal, pConnectionFactory){
	IA20_TRACER;
}
/*************************************************************************/
AsyncServer::~AsyncServer() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
}
