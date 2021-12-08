/*
 * File: IA20-CommonLib/src/commonlib/time/Date.cpp
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
#include "Date.h"
#include "DateTime.h"

#include <ia20/commonlib/commonlib.h>

namespace IA20 {

/*************************************************************************/
Date::Date(bool bCurrentSystemDate):Timestamp(bCurrentSystemDate){}
/*************************************************************************/
Date::Date(const Timestamp& other):Timestamp(other){}
Date::Date(const Date& other):Timestamp(other){}
Date::Date(const DateTime& other):Timestamp(other){}
/*************************************************************************/
Date::Date(const char *sValue){
	IA20_TRACER;
	String strValue(sValue);
	fromDateString(strValue);
}
/*************************************************************************/
Date::Date(const String& strValue){
	IA20_TRACER;
	fromDateString(strValue);
}
/*************************************************************************/
Date::~Date() throw(){}
/*************************************************************************/
String Date::toString()const{
	return toDateString();
}
/*************************************************************************/
void Date::fromString(const String& strValue){
	fromDateString(strValue);
}
/*************************************************************************/

}
