/*
 * File: IA20-CommonLib/src/commonlib/time/DateTime.cpp
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
#include "DateTime.h"
#include "Time.h"
#include "Date.h"

#include <ia20/commonlib/commonlib.h>

namespace IA20 {

/*************************************************************************/
DateTime::DateTime(bool bCurrentSystemDate):Timestamp(bCurrentSystemDate){}
/*************************************************************************/
DateTime::DateTime(const Timestamp& other):Timestamp(other){}
DateTime::DateTime(const DateTime& other):Timestamp(other){}
DateTime::DateTime(const Date& other):Timestamp(other){}
DateTime::DateTime(const Time& other):Timestamp(other){}
/*************************************************************************/
DateTime::DateTime(const char *sValue){
	IA20_TRACER;
	std::string strValue(sValue);
	fromDateTimeString(strValue);
}
/*************************************************************************/
DateTime::DateTime(const std::string& strValue){
	IA20_TRACER;
	fromString(strValue);
}
/*************************************************************************/
DateTime::DateTime(const std::string& strValue, const std::string& strFmt){
	IA20_TRACER;
	Timestamp::fromString(strValue, strFmt);
}
/*************************************************************************/
DateTime::DateTime(const std::string& strValue, const std::string& strFmt, bool toLocalTime){
	IA20_TRACER;
	Timestamp::fromString(strValue, strFmt, toLocalTime);
}
/*************************************************************************/
DateTime::~DateTime() throw(){}
/*************************************************************************/
std::string DateTime::toString()const{
	return toDateTimeString();
}
/*************************************************************************/
void DateTime::fromString(const std::string& strValue){
	fromDateTimeString(strValue);
}
/*************************************************************************/

}
