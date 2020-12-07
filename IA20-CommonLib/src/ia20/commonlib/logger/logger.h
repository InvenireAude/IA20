/*
 * File: IA20-CommonLib/src/commonlib/logger/logger.h
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

#ifndef IA20LOGGER_H_
#define IA20LOGGER_H_

#include "./LogLevel.h"
#include "./Logger.h"
#include "./Tracer.h"
#include "./StackTrace.h"
#include "../memory/ma/Mallocator.h"

#include <sstream>
#include <string>

#ifdef __IA20_NO_TRACER__
#define IA20_TRACER
#else
#define IA20_TRACER IA20::Tracer _IA20_tracer(__FUNCTION__,__FILE__,__LINE__);
#endif

#define IA20_LOG(i,s)              \
 if(i){ \
	BareStringStream ss; ss<<s;       \
	IA20::Logger::GetInstance()->addEntry(__FUNCTION__,__FILE__,__LINE__,ss.str().c_str()); \
	};

#define IA20_THROW(e)              \
  {IA20_LOG(::IA20::LogLevel::INSTANCE.isExceptionThrow(),"Exception !!!"); \
   throw e; }


#define IA20_REPORT_EXCEPTION(e)                         \
{  if(::IA20::LogLevel::INSTANCE.isReportedExceptions()){  \
	 IA20::Logger::GetInstance()->addEntry(__FUNCTION__,__FILE__,__LINE__,e.toString().c_str()); \
	}; \
 }



#endif /*IA20TYPES_H_*/
