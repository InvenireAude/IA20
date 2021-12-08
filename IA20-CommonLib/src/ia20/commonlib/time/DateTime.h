/*
 * File: IA20-CommonLib/src/commonlib/time/DateTime.h
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
#ifndef _IA20_DateTime_H_
#define _IA20_DateTime_H_

#include "Timestamp.h"

namespace IA20 {

class Date;
class Time;

/*************************************************************************/
/** The DateTime class.
 *
 */
class DateTime : public Timestamp{
public:

	DateTime(bool bCurrentSystemDate=false);
	DateTime(const Timestamp& other);
	DateTime(const DateTime& other);
	DateTime(const Date& other);
	DateTime(const Time& other);
	DateTime(const String& strValue);
	DateTime(const String& strValue, const String& strFmt);
	DateTime(const String& strValue, const String& strFmt, bool toLocalTime);
	DateTime(const char* sValue);

	~DateTime()throw();

	String toString()const;

	void fromString(const String& strValue);
};
/*************************************************************************/
}

#endif /* _IA20_DateTime_H_ */

