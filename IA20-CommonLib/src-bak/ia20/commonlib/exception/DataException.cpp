/*
 * File: IA20-CommonLib/src/commonlib/exception/DataException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
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
#include "DataException.h"

#include "../logger/logger.h"
#include "../ui/UserMessage.h"
#include "../ui/Messages.h"

#include <errno.h>
#include <string.h>

namespace IA20{

/*************************************************************************/
DataException::DataException(const std::string& strInfo){
	IA20_TRACER;
	setInfo(strInfo);
	UserMessage(Messages::MSGE_InternalError)<<strInfo;
}
/*************************************************************************/
DataException::DataException(){
	IA20_TRACER;
	std::string strInfo;
	UserMessage(Messages::MSGE_InternalError)<<strInfo;
}

/*************************************************************************/
DataException::~DataException() throw(){
	IA20_TRACER;

}
/*************************************************************************/
const char* DataException::getName(){
	IA20_TRACER;
	return "DataException";
}

/*************************************************************************/
} /* namespace IA20 */
