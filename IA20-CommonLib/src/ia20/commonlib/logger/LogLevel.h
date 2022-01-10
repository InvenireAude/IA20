/*
 * File: IA20-CommonLib/src/commonlib/logger/LogLevel.h
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef _IA20_LogLevel_H_
#define _IA20_LogLevel_H_


#include <iostream>


namespace IA20{

/*************************************************************************/
/** The LogLevel class.
 */

class LogLevelBase {
public:

		inline bool isSystem()      { return this->bIsSystem;       };
		inline bool isError()       { return this->bIsError;        };
		inline bool isInfo()        { return this->bIsInfo;         };
		inline bool isDetailedInfo(){ return this->bIsDetailedInfo; };
		inline bool isTrace()       { return this->bIsTrace;        };
		inline bool isMemory()      { return this->bIsMemory;       };
		inline bool isDspMsg()      { return this->bIsDspMsg;           };
		inline bool isStackTrace()  { return this->bIsStackTrace;       };
		inline bool isMemoryTrace() { return this->bIsMemoryTrace;      };
		inline bool isProfile ()    { return this->bIsProfile;        };
		inline bool isExceptionThrow()       { return this->bIsExceptionThrow;      };
		inline bool isReportedExceptions()   { return this->bIsReportedExceptions; };
		inline bool isData()        { return this->bIsData;       };
		inline bool isLogic()        { return this->bIsLogic;       };

	/*************************************************************************/
    static const char* CLevelEnv_GLOBAL;

		LogLevelBase(const char *sEnvVarName,
					 const char *sEnvSecondaryVarName = CLevelEnv_GLOBAL);

		virtual ~LogLevelBase();

/*************************************************************************/
 //TODO (L) protected ?
	 bool bIsSystem;
	 bool bIsError;
	 bool bIsInfo;
	 bool bIsDetailedInfo;
	 bool bIsTrace;
	 bool bIsMemory;
	 bool bIsDspMsg;
	 bool bIsStackTrace;
	 bool bIsException;
	 bool bIsReportedExceptions;
	 bool bIsExceptionThrow;
	 bool bIsMemoryTrace;
	 bool bIsProfile;
	 bool bIsData;
	 bool bIsLogic;
};

/*************************************************************************/

class LogLevel : public LogLevelBase {
	public:

	static LogLevel INSTANCE; // The global instance.

  static const char* CLevelEnv_COMMONLIB;

  LogLevel():
    LogLevelBase(CLevelEnv_COMMONLIB){};

};


/*************************************************************************/



}/* namespace IA20 */

#endif /*LOGGER_H_*/


