/*
 * File: Acceptor.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Acceptor.h"
#include "Listener.h"
#include "Server.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
Acceptor::Acceptor(URing::IO::TCP::AsyncServer* pAsyncServer, 			 
			       Listener *pListener):
      URing::IO::TCP::AsyncServer::Acceptor(pListener->getRingHandler(), pAsyncServer),
	  pListener(pListener){};
/*************************************************************************/
Acceptor::~Acceptor(){
 
}
/*************************************************************************/
void Acceptor::handleImpl(Net::Conn::TCP::FileHandle* pFileHandle){
	
	  pListener->createServer(pFileHandle);
      prepare();
  }
/*************************************************************************/
}
}
}
