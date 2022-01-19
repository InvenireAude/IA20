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

	virtual ~ActivityStore() throw();

  ActivityStore(IndexType iSize = 1000);

   Activity* createActivity( Subscription::HandleType   mSubscriptionHandle,
                             Topic::FullTopicNameHandle aTopicNameHandle,
                             Message::HandleType        mMessageHandle,
                             Listener::Task::Command    iCommand,
                             uint8_t      iQoS);

    inline Activity* back()const{
      
      if(lstPendingActivities.empty())
        IA20_THROW(ItemNotFoundException("iNumActivities == 0"));

      return lstPendingActivities.back();

    }

    inline void next(){
        lstPendingActivities.pop_back();
    }

    inline bool hasActivities()const{
      return !lstPendingActivities.empty();
    }


    inline void dispose(Activity *pActivity){
      //TODO ?? remove from pending list ??
      new (pActivity)Activity();
    }

protected:

  Activity* tActivites;

  IndexType iHead;
  IndexType iSize;

  typedef std::list<Activity*> ActivityList;
  ActivityList         lstPendingActivities;

  IndexType iNumActivites;
  
  Activity::SequenceType  iNextSequence;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ActivityStore_H_ */
