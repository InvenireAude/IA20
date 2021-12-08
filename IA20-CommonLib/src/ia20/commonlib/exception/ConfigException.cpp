/*
 * File: IA20-CommonLib/src/commonlib/exception/ConfigException.cpp
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
#include "ConfigException.h"

#include "../memory/memory.h"
#include "../logger/logger.h"
#include "../ui/UserMessage.h"
#include "../ui/Messages.h"

#include <errno.h>
#include <string.h>

namespace IA20{

/*************************************************************************/
ConfigException::ConfigException(const char *sInfo,
								 const char *sFileName,
								 int         iLineNo){
	IA20_TRACER;
	StringStream ssInfo;
	ssInfo<<sInfo;

	if(sFileName != NULL)
		ssInfo<<",File:"<<sFileName;

	if(iLineNo > 0)
			ssInfo<<" ["<<iLineNo<<"]";

	setInfo(ssInfo.str());
	UserMessage(Messages::MSGE_ConfigError)<<getInfo();
}

/*************************************************************************/
ConfigException::ConfigException(const String& strInfo){

	IA20_TRACER;

	setInfo(strInfo);
	UserMessage(Messages::MSGE_ConfigError)<<strInfo;
}


/*************************************************************************/
ConfigException::ConfigException(const String& strInfo,
							     const String& strFileName,
							     int iLineNo){
	IA20_TRACER;

	StringStream ssInfo;
	ssInfo<<strInfo;

	ssInfo<<" ,File:"<<strFileName;

	if(iLineNo > 0)
		ssInfo<<" ["<<iLineNo<<"]";

	setInfo(ssInfo.str());
	UserMessage(Messages::MSGE_ConfigError)<<strInfo;
}

/*************************************************************************/
ConfigException::~ConfigException() throw(){
	IA20_TRACER;

}
/*************************************************************************/
const char* ConfigException::getName(){
	IA20_TRACER;
	return "ConfigException";
}

/*************************************************************************/
} /* namespace IA20 */
