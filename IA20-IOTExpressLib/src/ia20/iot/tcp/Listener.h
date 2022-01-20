/*
 * File: Listener.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TCP_Listener_H_
#define _IA20_IOT_TCP_Listener_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>

#include "../Listener.h"

namespace IA20 {
namespace IOT {
namespace TCP {
class Server;
/*************************************************************************/
/** The Listener class.
 *
 */
class Listener : 
	public IOT::Listener,
	public Thread,
	public Runnable {
public:

	virtual ~Listener() throw();
	Listener(int iMaxConnections);

	virtual void run();

	void createServer(Net::Conn::TCP::FileHandle* pFileHandle);

	inline uint8_t* getContentPayload(Message::HandleType aMessageHandle)const{
		ContentMap::const_iterator it = hmContent.find(aMessageHandle);
		
		if(it == hmContent.end()){
			IA20_THROW(ItemNotFoundException("Content for: ")<<(int)aMessageHandle);
		}

		return it->second.pPayLoad;
	};

  virtual Memory::SharableMemoryPool *getMemoryPool()const{
    return ptrMemoryPoolHolder.get();
  }

protected:

  std::unique_ptr<Memory::SharableMemoryPool> ptrMemoryPoolHolder;

  std::list< std::unique_ptr<Server> > lstServers;
  typedef std::unordered_map< Connection::HandleType, Server*> ConnectionHandleMap;
  
  ConnectionHandleMap hmConnectionHandle;

  struct ContentEntry{
    int   iUsageCount;
    uint8_t *pPayLoad;
  };

  typedef std::map<Message::HandleType, ContentEntry> ContentMap;

  ContentMap hmContent;

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_TCP_Listener_H_ */
