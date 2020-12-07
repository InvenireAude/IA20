/*
 * File: IA20-CommonLib/src/commonlib/logger/FileSizeLimitedLogger.h
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

#ifndef _IA20_Logger_FileSizeLimitedLogger_H_
#define _IA20_Logger_FileSizeLimitedLogger_H_

#include "FileLogger.h"

namespace IA20{
namespace LoggerOutputs {


/*************************************************************************/
/** The FileSizeLimitedLogger class.
 */

class FileSizeLimitedLogger : public ::IA20::LoggerOutputs::FileLogger {

public:

	virtual ~FileSizeLimitedLogger();
	FileSizeLimitedLogger();

private:

	size_t iLogLines;
	size_t iLogMaxLines;
	int    iNumLogs;

	virtual void rotateLogs();
	void openLogFile();

	static const char* CEnvMaxLines;
	static const char* CEnvMaxFiles;
};

}
}
#endif /*_IA20_Logger_FileSizeLimitedLogger_H_*/

