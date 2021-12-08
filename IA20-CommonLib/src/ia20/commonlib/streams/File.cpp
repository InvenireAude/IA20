/*
 * File: IA20-CommonLib/src/commonlib/streams/File.cpp
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
#include "File.h"
#include "../memory/memory.h"


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


namespace IA20 {

File::File(const String& strName) :
strName(strName){
	IA20_TRACER;

}

File::~File() {
	IA20_TRACER;

}

String& File::getName(){
	IA20_TRACER;
	return strName;
}

void File::Remove(const String& strName){
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isSystem(),"Removing: ["<<strName<<"]");

	if(unlink(strName.c_str())==-1)
		IA20_THROW(SystemException(strName));
}

bool File::Exists(const String& strName){
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isSystem(),"Checking: ["<<strName<<"]");

	struct stat stat;

	if(::stat(strName.c_str(), &stat) == 0)
		return true;

	if(errno == ENOENT || errno == ENOTDIR)
		return false;

	IA20_THROW(SystemException(strName));
}

}
