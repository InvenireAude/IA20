/*
 * File: Activity.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Activity_H_
#define _IA20_IOT_Activity_H_

#include <ia20/commonlib/commonlib.h>

#include "Subscription.h"
#include "Subscription.h"
#include "MessageStore.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Activity class.
 *
 */
class Activity {
    public:


    Activity(int iListenerId,
             Subscription::HandleType   sSubscriptionHandle,
             Message::HandleType        aMessageHandle):
        aSubscriptionHandle(sSubscriptionHandle),
        aMessageHandle(aMessageHandle),
        iListenerId(iListenerId){}


      inline Subscription::HandleType getSubscriptionHandle()const{
        return aSubscriptionHandle;
      };
      
      inline Message::HandleType getMessageHandle()const{
        return aMessageHandle;
      };
      
      inline int getListenerId()const{
        return iListenerId;
      };

    protected:
     
     Subscription::HandleType   aSubscriptionHandle;
     Message::HandleType        aMessageHandle;
    int iListenerId;
  };
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Activity_H_ */
