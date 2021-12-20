/*
 * File: MockerThread.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Mocker_MockerThread_H_
#define _IA20_IOT_Mocker_MockerThread_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
/** The MockerThread class.
 *
 */
class MockerThread : public Thread, public Runnable{
public:

	virtual ~MockerThread() throw();
	MockerThread();

protected:

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_MockerThread_H_ */
