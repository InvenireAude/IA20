/*
 * File: IA20-CommonLib/src/commonlib/logger/FileSizeLimitedLogger.cpp
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

#include "FileSizeLimitedLogger.h"

#include <ia20/commonlib/tools/TypeTools.h>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace IA20{
namespace LoggerOutputs {

const char* FileSizeLimitedLogger::CEnvMaxLines("IA20_DBG_LOG_MAXLINES");
const char* FileSizeLimitedLogger::CEnvMaxFiles("IA20_DBG_LOG_MAXFILES");

/*************************************************************************/
FileSizeLimitedLogger::FileSizeLimitedLogger():
		iLogLines(0),
		iLogMaxLines(10000),
		iNumLogs(3){

	if(getenv(CEnvMaxLines))
		iLogMaxLines = atoi(getenv(CEnvMaxLines));

	if(getenv(CEnvMaxFiles))
		iNumLogs = atoi(getenv(CEnvMaxFiles));

}

/*************************************************************************/
FileSizeLimitedLogger::~FileSizeLimitedLogger(){
}
/*************************************************************************/
void FileSizeLimitedLogger::openLogFile(){


	FileLogger::openLogFile();

	iLogLines = 0;

	if(ofLog.good()){
		iLogLines= std::count(std::istreambuf_iterator<char>(ofLog),
			             std::istreambuf_iterator<char>(), '\n');
	}

}

/*************************************************************************/
void FileSizeLimitedLogger::rotateLogs(){

	if(!ofLog.is_open() || !ofLog.good())
		return;

	if(++iLogLines < iLogMaxLines)
		return;

	ofLog.close();

	char sFilename1[256];
	char sFilename2[256];

	for(int iIdx = iNumLogs; iIdx > 0; iIdx--){
		snprintf(sFilename2,256,"%s.%d",sLogBasename,iIdx);

		if(iIdx == iNumLogs)
			unlink(sFilename2);

		if(iIdx > 1)
			snprintf(sFilename1,256,"%s.%d",sLogBasename,iIdx - 1);
		else
			snprintf(sFilename1,256,"%s",sLogBasename);

		rename(sFilename1,sFilename2);

	}

	openLogFile();
}

/*************************************************************************/
}
}/*namespace IA20 */
