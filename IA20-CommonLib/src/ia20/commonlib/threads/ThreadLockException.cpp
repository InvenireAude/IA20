/*
 * File: IA20-CommonLib/src/commonlib/threads/ThreadLockException.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ThreadLockException.h"

#include "../logger/logger.h"

namespace IA20 {

/*************************************************************************/
ThreadLockException::ThreadLockException()throw(){}
/*************************************************************************/
ThreadLockException::ThreadLockException(const String& strInfo, int iErrCode) throw(){
	IA20_TRACER;
	StringStream ssInfo;
	ssInfo<<strInfo<<":"<<iErrCode;
	this->setInfo(ssInfo.str());
}

/*************************************************************************/
ThreadLockException::~ThreadLockException() throw(){
	IA20_TRACER;
}

/*************************************************************************/
const char* ThreadLockException::getName(){
	return "ThreadLockException";
}
/*************************************************************************/
ThreadLockTimeoutException::ThreadLockTimeoutException()throw(){};
/*************************************************************************/
const char* ThreadLockTimeoutException::getName(){
	return "ThreadLockTimeoutException";
};

}
