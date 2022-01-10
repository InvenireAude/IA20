/*
 * File: Activity.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Activity_H_
#define _IA20_IOT_Activity_H_

#include <ia20/commonlib/commonlib.h>

#include "Connection.h"
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
             Connection::HandleType aConnectionHandle,
             Message::HandleType    aMessageHandle):
        aConnectionHandle(aConnectionHandle),
        aMessageHandle(aMessageHandle),
        iListenerId(iListenerId){}


      inline Connection::HandleType getConnectionHandle()const{
        return aConnectionHandle;
      };
      
      inline Message::HandleType getMessageHandle()const{
        return aMessageHandle;
      };
      
      inline int getListenerId()const{
        return iListenerId;
      };

    protected:
     
     Connection::HandleType   aConnectionHandle;
     Message::HandleType      aMessageHandle;
    int iListenerId;
  };
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Activity_H_ */
