/*
 * File: Server.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_Server_H_
#define _IA20_IOT_TCP_Server_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>

#include <ia20/iot/memory/StreamBufferList.h>

#include <string.h>
#include <inttypes.h>

#include "../Listener.h"
#include "Context.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
/** The Server class.
 *
 */

class Listener;
class Server : 
	public URing::IO::ReadHandler, 
	public URing::IO::WriteHandler, 
	public URing::IO::ShutdownHandler {
 
  public:
    
	Server(Net::Conn::TCP::FileHandle* pFileHandle,
		    const Listener *pListener);

	~Server()throw();

  static const String CStrData;

  String strData;

  size_t iMessages = 0;

  void sendMessage(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask);
  void publishMessage(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask);

  protected:

  virtual void handleRead(off_t iDataLen);
  virtual void handleWrite(off_t iDataLen);
  virtual void handleShutdown(int iResult);


 protected:

  void sendAndRead(const String& msg){
      MiscTools::HexToBinary(msg, WriteHandler::iovec.iov_base, 4000);
	  std::cerr<<"Seding data: ["<<msg<<"]"<<std::endl;
      WriteHandler::iovec.iov_len = msg.length()/2;
      WriteHandler::prepare();
      ReadHandler::prepare();
  }

  const Listener *pListener;
  std::unique_ptr<Net::Conn::TCP::FileHandle> ptrFileHandle;

  Context ctx;

  std::unique_ptr<uint8_t> ptrInputBuffer;
  int    	               iBufferDataLength;
  std::unique_ptr<uint8_t> ptrOutputBuffer;

  Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task> ptrTask;

  Connection::HandleType aConnectionHandle;

  void prepareRead();
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_Server_H_ */
