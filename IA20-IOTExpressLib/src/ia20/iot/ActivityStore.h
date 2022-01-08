/*
 * File: ActivityStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ActivityStore_H_
#define _IA20_IOT_ActivityStore_H_

#include <ia20/commonlib/commonlib.h>


#include "Subscription.h"
#include "ActionsStore.h"

namespace IA20 {
namespace IOT {
namespace MQTT {
  class Message;
}

/*************************************************************************/
/** The ActivityStore class.
 *
 */

class SubscriptionStore;
class ActionsStore;

class ActivityStore {
protected:
  typedef uint32_t IndexType;

public:

  class Activity {
    public:


    Activity(Subscription::HandleType mSubscriptionHandle,
     ActionsStore::Action::HandleType mActionHandle):
        mSubscriptionHandle(mSubscriptionHandle),
        mActionHandle(mActionHandle){}


      inline Subscription::HandleType getSubscriptionHandle()const{
        return mSubscriptionHandle;
      };
      inline ActionsStore::Action::HandleType getActionHandle()const{
        return mActionHandle;
      };

    protected:
     
     Subscription::HandleType         mSubscriptionHandle;
     ActionsStore::Action::HandleType mActionHandle;

  };

	virtual ~ActivityStore() throw();

  ActivityStore(
    IndexType          iSize = 1000);


   inline void createActivity(Subscription::HandleType  mSubscriptionHandle,
                       ActionsStore::Action::HandleType mActionHandle){
      
        if(iNumActivites == iSize)
            IA20_THROW(ItemNotFoundException("iNumActivities == iSize"));
        
        IA20_LOG(true, "New Activity ["<<iHead<<"], Sub: "
          <<(void*)(long)mSubscriptionHandle<<":"
          <<(void*)(long)mActionHandle);

        Activity* pActivity = tActivites + iHead++;
        new(pActivity) Activity(mSubscriptionHandle, mActionHandle);

        if(iHead == iSize){
          iHead = 0;
        }
      }

    inline Activity* back()const{
      
      if(iNumActivites == 0)
        IA20_THROW(ItemNotFoundException("iNumActivities == 0"));

      return tActivites + ((iHead - iNumActivites + iSize) % iSize);

    }

    void next(){

       if(iNumActivites == 0)
         IA20_THROW(ItemNotFoundException("iNumActivities == 0"));

       iNumActivites --;
    }

protected:

  Activity* tActivites;

  IndexType iSize;
  IndexType iHead;
  IndexType iNumActivites;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ActivityStore_H_ */
