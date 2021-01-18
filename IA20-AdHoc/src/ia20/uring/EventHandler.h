/*
 * File: EventHandler.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _EventHandler_H_
#define _EventHandler_H_

#include <ia20/commonlib/commonlib.h>
#include <liburing.h>

/*************************************************************************/
/** The EventHandler class.
 *
 */
class EventHandler {
public:

	virtual ~EventHandler() throw();

  virtual void handle(int iResult) = 0;

  protected:

  EventHandler();
};
/*************************************************************************/

#endif /* _EventHandler_H_ */
