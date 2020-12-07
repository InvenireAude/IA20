/*
 * File: IA20-CommonLib/src/commonlib/time/Time.h
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
#ifndef _IA20_Time_H_
#define _IA20_Time_H_

#include "Timestamp.h"

namespace IA20 {

class DateTime;

/*************************************************************************/
/** The Time class.
 *
 */
class Time : public Timestamp{
public:

	Time(bool bCurrentSystemDate=false);
	Time(const Timestamp& other);
	Time(const DateTime& other);
	Time(const Time& other);
	Time(const String& strValue);
	Time(const char *sValue);

	~Time()throw();

	String toString()const;

	void fromString(const String& strValue);

};
/*************************************************************************/
}

#endif /* _IA20_Time_H_ */

