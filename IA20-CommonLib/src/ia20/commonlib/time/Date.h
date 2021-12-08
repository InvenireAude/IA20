/*
 * File: IA20-CommonLib/src/commonlib/time/Date.h
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
#ifndef _IA20_Date_H_
#define _IA20_Date_H_

#include "Timestamp.h"

namespace IA20 {

class DateTime;

/*************************************************************************/
/** The Date class.
 *
 */
class Date : public Timestamp{
public:

	Date(bool bCurrentSystemDate=false);
	Date(const Timestamp& other);
	Date(const Date& other);
	Date(const DateTime& other);
	Date(const String& strValue);
	Date(const char *sValue);

	~Date()throw();

	String toString()const;

	void fromString(const String& strValue);

};
/*************************************************************************/
}

#endif /* _IA20_Date_H_ */

