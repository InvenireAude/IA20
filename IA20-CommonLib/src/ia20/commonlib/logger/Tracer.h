/*
 * File: IA20-CommonLib/src/commonlib/logger/Tracer.h
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

#ifndef _IA20_Tracer_H_
#define _IA20_Tracer_H_

#include "LogLevel.h"
#include "Logger.h"

namespace IA20{

/*************************************************************************/
/** The Tracer class.
 */

class Tracer{
public:

 inline	Tracer(const char *sFun, const char *sFile, int iLine):
       sFun(sFun),
       sFile(sFile),
       iLine(iLine){

            if(IA20::LogLevel::INSTANCE.isTrace()){
              IA20::Logger::GetInstance()->addEntry(sFun,sFile,iLine,">> Entering");
              };

       }

	~Tracer(){

       if(IA20::LogLevel::INSTANCE.isTrace()){
          IA20::Logger::GetInstance()->addEntry(sFun,sFile,iLine,"<< Leaving");
        };
  }

   protected:
	   const char *sFun;
	   const char *sFile;
	   int        iLine;
};

}

#endif /*TRACER_H_*/
