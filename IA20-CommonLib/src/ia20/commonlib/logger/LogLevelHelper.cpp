/*
 * File: IA20-CommonLib/src/commonlib/logger/LogLevelHelper.cpp
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

/*
 * Do not use the STL and other advanced structures here as they may not be initialized properly.
 * For the same reason we do not report errors and log anything here.
 */

#include <iostream>

#include "LogLevel.h"
#include "LogLevelHelper.h"

#include <string.h>

#define IA20_LOGLEVEL_MAXWORD_LEN 32
#define IA20_DBGLEVEL_CHECKWORD(w,s,v) \
	if(strncmp(sWord,(w),IA20_LOGLEVEL_MAXWORD_LEN)==0) \
	  ptrLogLevel->s  = (v);

namespace IA20{


static inline void _checkWord(const char* sName, const char* sWord, bool& bField){

  bool bValue;

  switch(sWord[0]){
    case '+':
      bValue = true;
     break;
    case '-':
      bValue = false;
      break;
    default:
     return;
  }

  if(strncmp(sWord + 1,sName, IA20_LOGLEVEL_MAXWORD_LEN) == 0){
    bField = bValue;
  }

};

void LogLevelHelper::ReadWordSettings(LogLevelBase   *ptrLogLevel, const char *sWord){

	if(ptrLogLevel == NULL || sWord == NULL)
		return;

  _checkWord("system", sWord, ptrLogLevel->bIsSystem);
  _checkWord("error", sWord, ptrLogLevel->bIsError);
  _checkWord("info", sWord, ptrLogLevel->bIsInfo);
  _checkWord("trace", sWord, ptrLogLevel->bIsTrace);
  _checkWord("details", sWord, ptrLogLevel->bIsDetailedInfo);
  _checkWord("memory", sWord, ptrLogLevel->bIsMemory);
  _checkWord("msg", sWord, ptrLogLevel->bIsDspMsg);
  _checkWord("stacktrace", sWord, ptrLogLevel->bIsStackTrace);
  _checkWord("throw", sWord, ptrLogLevel->bIsExceptionThrow);
  _checkWord("reportedexception", sWord, ptrLogLevel->bIsReportedExceptions);
  _checkWord("memorytrace", sWord, ptrLogLevel->bIsMemoryTrace);
  _checkWord("profile", sWord, ptrLogLevel->bIsProfile);
  _checkWord("data", sWord, ptrLogLevel->bIsData);
  _checkWord("logic", sWord, ptrLogLevel->bIsLogic);

}

/*************************************************************************/
void LogLevelHelper::ReadSettings(LogLevelBase   *ptrLogLevel,
								  const char *sSettings){

	if(ptrLogLevel == NULL || sSettings == NULL)
		return;

	int iIdx;
	char sBuffer[IA20_LOGLEVEL_MAXWORD_LEN + 1];
	char *sCur;

	while(*sSettings != 0){

		iIdx = 0;
		sCur=sBuffer;

		while(*sSettings != 0 && *sSettings != ','){

			if(iIdx < IA20_LOGLEVEL_MAXWORD_LEN)
				*(sCur++)=*sSettings;

			iIdx++;
			sSettings++;

		}/* WHILE: */

		*sCur = 0;

		if(*sSettings == ',')
			sSettings++;

		if(iIdx > 0 && iIdx < IA20_LOGLEVEL_MAXWORD_LEN)
			ReadWordSettings(ptrLogLevel, sBuffer);

	}/* WHILE */
}

/*************************************************************************/
void LogLevelHelper::SetDefaults(LogLevelBase   *ptrLogLevel){

	if(ptrLogLevel == NULL)
		return;

	ptrLogLevel->bIsSystem         = false;
	ptrLogLevel->bIsError          = true;
	ptrLogLevel->bIsInfo           = false;
  ptrLogLevel->bIsData           = false;
  ptrLogLevel->bIsLogic          = false;
	ptrLogLevel->bIsDetailedInfo   = false;
  ptrLogLevel->bIsMemoryTrace    = false;
  ptrLogLevel->bIsProfile        = false;
	ptrLogLevel->bIsMemory         = false;
	ptrLogLevel->bIsStackTrace     = false;
	ptrLogLevel->bIsExceptionThrow = false;
	ptrLogLevel->bIsDspMsg         = true;
	ptrLogLevel->bIsReportedExceptions     = true;


}
}/*namespace IA20 */

