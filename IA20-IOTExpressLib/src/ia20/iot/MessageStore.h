/*
 * File: MessageStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_MessageStore_H_
#define _IA20_IOT_MessageStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/StreamBufferList.h>
#include "Message.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The MessageStore class.
 *
 */
class MessageStore {
public:

	virtual ~MessageStore() throw();
	MessageStore();

  const Message* createMessage(Memory::StreamBufferList::Reader& reader , uint32_t iDataLength, uint8_t iQoS);
  const Message* lookup(Message::HandleType aHandle)const;
  void           dispose(const Message* pMessage);

protected:


  //TODO temporary impl

  struct Deleter { 
    void operator() (Message* p) {
      	IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "!!!: "<<"free pMessage: "<<(void*)p);
        std::free(p);
    }
  };

  typedef std::map<Message::HandleType, std::unique_ptr<Message, Deleter> > MessagesMap;

  MessagesMap hmMessages;
  Message::HandleType iNextHandle;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_MessageStore_H_ */
