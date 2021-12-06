/*
 * File: IA20-CommonLib/src/commonlib/streams/Directory.cpp
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
#include "Directory.h"
#include "../logger/logger.h"
#include "../exception/SystemException.h"

#include <sys/stat.h>
#include <sys/types.h>


namespace IA20 {

Directory::Directory() {
	IA20_TRACER;

}

Directory::~Directory() {
	IA20_TRACER;

}

void Directory::CreateDirectory(const String& strDirectoryName){
	IA20_TRACER;
	//TODO  (L) -p like functionality
	if(mkdir(strDirectoryName.c_str(),0777) < 0){
		IA20_THROW(SystemException(errno));
	}

}

void Directory::CreateDirectoryIfNecessary(const String& strDirectoryName){
	IA20_TRACER;

	mkdir(strDirectoryName.c_str(),0777);

	if(mkdir(strDirectoryName.c_str(),0777) < 0 && errno != EEXIST){
		IA20_THROW(SystemException(errno));
	}

}


}
