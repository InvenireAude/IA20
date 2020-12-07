/*
 * File: IA20-CommonLib/src/commonlib/logger/FileLogger.cpp
 *
 * Copyright (C) 2017, Albert Krzymowski
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#include "FileLogger.h"

#include <iomanip>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace IA20{
namespace LoggerOutputs {

const char* FileLogger::CEnvFileBasename("IA20_DBG_LOGFILE");
/*************************************************************************/
FileLogger::FileLogger():
		sLogBasename(getenv(CEnvFileBasename)){

	if(sLogBasename){
		openLogFile();
	}
}
/*************************************************************************/
FileLogger::~FileLogger(){
}
/*************************************************************************/
void FileLogger::openLogFile(){

	ofLog.open(sLogBasename,std::fstream::in | std::fstream::out | std::fstream::app);

	if(ofLog.good())
		setup(&ofLog);

}
/*************************************************************************/
}
}/*namespace IA20 */
