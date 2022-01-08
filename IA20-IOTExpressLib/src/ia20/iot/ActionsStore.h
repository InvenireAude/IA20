/*
 * File: ActionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ActionsStore_H_
#define _IA20_IOT_ActionsStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/StreamBufferList.h>

#include <map>

namespace IA20 {
namespace IOT {
namespace MQTT {
  class Message;
}

/*************************************************************************/
/** The ActionsStore class.
 *
 */
class ActionsStore {
public:

  class Action {
    public:
    
    inline const uint8_t* getData()const{
      return reinterpret_cast<const uint8_t*>(this+1);
    }

    typedef uint32_t HandleType;

    inline HandleType getHandle()const{
      return iHandle;
    }

    inline Action(HandleType iHandle):iHandle(iHandle){};

    protected:

    HandleType iHandle;

    inline uint8_t* getDataPointer(){
      return reinterpret_cast<uint8_t*>(const_cast<Action*>(this+1));
    }

    static const size_t CAlignedSize;

    static size_t ComputeRequiredMemory(uint32_t iDataLen){
      return CAlignedSize + iDataLen;
    };

    friend class ActionsStore;
  };

  
	virtual ~ActionsStore() throw();
  ActionsStore();

  const Action* createAction(Memory::StreamBufferList::Reader& reader , uint32_t iDataLength);
  const Action* lookup(Action::HandleType aHandle)const;
  void          dispose(const Action* pAction);

protected:


  //TODO temporary impl

  struct Deleter { 
    void operator() (Action* p) {
        std::free(p);
    }
  };

  typedef std::map<Action::HandleType, std::unique_ptr<Action, Deleter> > ActionsMap;

  ActionsMap hmActions;
  Action::HandleType iNextHandle;
  
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ActionsStore_H_ */
