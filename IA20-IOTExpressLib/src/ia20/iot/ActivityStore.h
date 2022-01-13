/*
 * File: ActivityStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ActivityStore_H_
#define _IA20_IOT_ActivityStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/FixedObjectsPool.h>


#include "Subscription.h"
#include "Activity.h"
#include "MessageStore.h"

namespace IA20 {
namespace IOT {
namespace MQTT {
  class Message;
}

/*************************************************************************/
/** The ActivityStore class.
 *
 */

class ConnectionStore;
class MessageStore;

class ActivityStore {
protected:
  typedef uint32_t IndexType;

public:

  typedef Memory::FixedObjectsPool<Activity, 1000> ActivityPool;

	virtual ~ActivityStore() throw();

  ActivityStore(IndexType iSize = 1000);

   inline void createActivity(Subscription::HandleType  mSubscriptionHandle,
                              Message::HandleType       mMessageHandle){
      
        if(iNumActivites == iSize)
            IA20_THROW(ItemNotFoundException("iNumActivities == iSize"));
        
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "New Activity ["<<iHead<<"], Sub: "
          <<(void*)(long)mSubscriptionHandle<<":"
          <<(void*)(long)mMessageHandle);

        Activity* pActivity = ActivityPool::New(0, mSubscriptionHandle, mMessageHandle);
        tActivites[iHead++] = pActivity;

        if(iHead == iSize){
          iHead = 0;
        }

        iNumActivites++;
      }

    inline Activity* back()const{
      
      if(iNumActivites == 0)
        IA20_THROW(ItemNotFoundException("iNumActivities == 0"));

      return tActivites[(iHead - iNumActivites + iSize) % iSize];

    }

    inline void next(){

       if(iNumActivites == 0)
         IA20_THROW(ItemNotFoundException("iNumActivities == 0"));

       ActivityPool::Free(tActivites[(iHead - iNumActivites + iSize) % iSize]);
       
       iNumActivites--;
    }

    inline bool hasActivities()const{
      return iNumActivites > 0;
    }

protected:

  Activity** tActivites;

  IndexType iSize;
  IndexType iHead;
  IndexType iNumActivites;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ActivityStore_H_ */
