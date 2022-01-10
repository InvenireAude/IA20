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

	virtual ~Listener() throw();
	Listener(std::unique_ptr<RingType::Interface>&& ptrInterface);

  void sendMessage(const String& strHex);

  void serve();

protected:

  void run();

  std::unique_ptr<Memory::SharableMemoryPool> ptrMemoryPoolHolder;

  struct ContentEntry{
    int   iUsageCount;
    uint8_t *pPayLoad;
  };

  typedef std::map<Tools::IdentifiedByHandle::HandleType, ContentEntry> ContentMap;

  ContentMap hmContent; 

  Connection::HandleType aConnectionHandle;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_Listener_H_ */
