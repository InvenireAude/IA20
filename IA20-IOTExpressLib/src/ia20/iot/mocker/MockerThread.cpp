/*
 * File: MockerThread.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "MockerThread.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
MockerThread::MockerThread():
Thread(this){
	IA20_TRACER;
}

/*************************************************************************/
MockerThread::~MockerThread() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
