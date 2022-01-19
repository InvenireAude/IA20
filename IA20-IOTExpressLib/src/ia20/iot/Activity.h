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
#include "Listener.h"
#include "Topic.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Activity class.
 *
 */
class Activity {
    public:

    typedef uint32_t SequenceType;
    
    enum State : uint8_t {
      ST_None        = 0,
      ST_SendPending  = 1,
      ST_AckPending   = 2
    };

    Activity():
      iSequence(0),
      iState(ST_None){};

    Activity(SequenceType iSequence,
             State        iState,
             Subscription::HandleType   sSubscriptionHandle,
             Topic::FullTopicNameHandle aTopicNameHandle,
             Message::HandleType        aMessageHandle,
             Listener::Task::Command    iCommand,
             uint8_t                    iQoS):
        iSequence(iSequence),
        iState(iState),
        aSubscriptionHandle(sSubscriptionHandle),
        aTopicNameHandle(aTopicNameHandle),
        aMessageHandle(aMessageHandle),
        iCommand(iCommand),
        iQoS(iQoS){}


      inline Subscription::HandleType getSubscriptionHandle()const{
        return aSubscriptionHandle;
      };
      
      inline Topic::FullTopicNameHandle getTopicNameHandle()const{
        return aTopicNameHandle;
      };

      inline Message::HandleType getMessageHandle()const{
        return aMessageHandle;
      };

      bool isEmpty(){
        return iState = ST_None;
      }

  
      inline uint8_t getQoS()const{
        return iQoS;
      }
      
     Listener::Task::Command    getCommand()const{
       return iCommand;
     }

    protected:

     SequenceType               iSequence;
     State                      iState;
     uint8_t                    iQoS;
     Listener::Task::Command    iCommand;
     Subscription::HandleType   aSubscriptionHandle;
     Message::HandleType        aMessageHandle;
     Topic::FullTopicNameHandle aTopicNameHandle;
  };
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Activity_H_ */
