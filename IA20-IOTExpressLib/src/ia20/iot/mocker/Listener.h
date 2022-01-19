/*
 * File: Listener.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Mocker_Listener_H_
#define _IA20_IOT_Mocker_Listener_H_

#include <ia20/commonlib/commonlib.h>

#include "../Listener.h"
#include "../Connection.h"

#include "MockerThread.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
/** The Listener class.
 *
 */
class Listener :
public MockerThread,
public IOT::Listener {
public:

  struct ConnectionEntry {
    std::list<String> lstWire;
    Connection::HandleType aConnectionHandle;
  };

  typedef std::vector<ConnectionEntry> ConnectionTab;

  const ConnectionTab getConnectionStates()const{
    return tabConnections;
  }

	virtual ~Listener() throw();
	Listener(int fdIn, int fdOut, int iMaxConnections = 1);

  void sendMessage(const String& strHex, int iConnectionId = 0);

  bool serve(bool bWait);
  void serveUntilEmptyQueue();
  
  void dump(std::ostream& os);

    virtual Memory::SharableMemoryPool *getMemoryPool()const{
    return ptrMemoryPoolHolder.get();
  }

protected:

  void run();

  std::unique_ptr<Memory::SharableMemoryPool> ptrMemoryPoolHolder;

  struct ContentEntry{
    int   iUsageCount;
    uint8_t *pPayLoad;
  };

  typedef std::map<Message::HandleType, ContentEntry> ContentMap;

  ContentMap hmContent;

  ConnectionTab  tabConnections;

  typedef std::map<Connection::HandleType, ConnectionEntry*> ConnectionHandleMap;

  ConnectionHandleMap hmConnectionHandles;

  int fdIn, fdOut;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_Listener_H_ */

