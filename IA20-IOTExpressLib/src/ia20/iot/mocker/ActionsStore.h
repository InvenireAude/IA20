/*
 * File: ActionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Mocker_ActionsStore_H_
#define _IA20_IOT_Mocker_ActionsStore_H_

#include <ia20/commonlib/commonlib.h>
#include "../ActionsStore.h"

#include "MockerThread.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
/** The ActionsStore class.
 *
 */
class ActionsStore :
public MockerThread,
public IOT::ActionsStore {
public:

	virtual ~ActionsStore() throw();
	ActionsStore();

  virtual void run();

protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_ActionsStore_H_ */
