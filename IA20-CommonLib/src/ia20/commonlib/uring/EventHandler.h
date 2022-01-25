/*
 * File: EventHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_EventHandler_H_
#define _IA20_URing_EventHandler_H_

#include <ia20/commonlib/commonlib.h>
#include <liburing.h>

namespace IA20 {
namespace URing {

class RingHandler;

/*************************************************************************/
/** The EventHandler class.
 *
 */
class EventHandler {
public:

	virtual ~EventHandler() throw();

  virtual void handle(int iResult) = 0;

  inline bool isInProgress()const{
    return bInProgress;    
  }

  protected:

    EventHandler(RingHandler* pRingHandler);
    RingHandler* pRingHandler;

  bool bInProgress;
};
/*************************************************************************/

}
}
#endif /* _IA20_URing_EventHandler_H_ */
