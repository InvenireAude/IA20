/*
 * File: ActivityStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Mocker_ActivityStore_H_
#define _IA20_IOT_Mocker_ActivityStore_H_

#include <ia20/commonlib/commonlib.h>
#include "../ActivityStore.h"

#include "MockerThread.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
/** The ActivityStore class.
 *
 */
class ActivityStore :
public MockerThread,
public IOT::ActivityStore {
public:

	virtual ~ActivityStore() throw();
	ActivityStore(
		
	);

  virtual void run();

protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_ActivityStore_H_ */
