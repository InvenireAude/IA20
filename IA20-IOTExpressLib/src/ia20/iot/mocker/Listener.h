/*
 * File: Listener.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Mocker_Listener_H_
#define _IA20_IOT_Mocker_Listener_H_

#include <ia20/commonlib/commonlib.h>

#include "../Listener.h"
#include "MockerThread.h"

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
/** The Listener class.
 *
 */
class Listener :
public MockerThread,
public IOT::Listener {
public:

	virtual ~Listener() throw();
	Listener(std::unique_ptr<RingType::Interface>&& ptrInterface);


  void sendMessage(const String& strHex);

protected:

  void run();

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Mocker_Listener_H_ */
