/*
 * File: IA20-CommonLib/src/commonlib/logger/TimeLimitedLogger.cpp
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

#include "TimeLimitedLogger.h"

#include <unistd.h>
#include <time.h>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>

namespace IA20{
namespace LoggerOutputs {

const char* TimeLimitedLogger::CEnvPeriod("IA20_DBG_LOG_PERIOD");

/*************************************************************************/
TimeLimitedLogger::TimeLimitedLogger():
		iPeriod(3600*24),
		tLastUpdate(0){

	tzset();
	tLastUpdate = time(NULL) + timezone;

	if(getenv(CEnvPeriod))
		iPeriod = atoi(getenv(CEnvPeriod));

	if(iPeriod < 60)
		iPeriod = 60;

}

/*************************************************************************/
TimeLimitedLogger::~TimeLimitedLogger(){
}
/*************************************************************************/
static time_t _getFileUpdateTime(const char *sFileName)
{
    struct stat fileAttr;
    if(stat(sFileName, &fileAttr) == 0)
    	return fileAttr.st_mtim.tv_sec + timezone;
    else
    	return time(NULL);
}
/*************************************************************************/
void TimeLimitedLogger::openLogFile(){


	FileLogger::openLogFile();


	if(ofLog.good())
		tLastUpdate = _getFileUpdateTime(sLogBasename);

}

/*************************************************************************/
void TimeLimitedLogger::rotateLogs(){

	if(!ofLog.is_open() || !ofLog.good())
		return;

	time_t tCurrentUpdate = time(NULL) + timezone;

	if(tLastUpdate / iPeriod * iPeriod == tCurrentUpdate / iPeriod * iPeriod)
		return;

	ofLog.close();

	char sTime[64];

	struct tm tmFileTime;
	time_t tFileTime = (tLastUpdate / iPeriod * iPeriod) - timezone;

	if(localtime_r(&tFileTime, &tmFileTime) != NULL && strftime(sTime,64,"%Y-%m-%d-%H:%M:%S",&tmFileTime) > 0){
			std::stringstream ssFilename;
			ssFilename<<sLogBasename<<"."<<sTime;
			rename(sLogBasename, ssFilename.str().c_str());
	}

	tLastUpdate = tCurrentUpdate;
	openLogFile();
}

/*************************************************************************/
}
}/*namespace IA20 */
