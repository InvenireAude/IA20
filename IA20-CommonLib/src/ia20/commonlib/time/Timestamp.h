/*
 * File: IA20-CommonLib/src/commonlib/time/Timestamp.h
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
#ifndef _IA20_Timestamp_H_
#define _IA20_Timestamp_H_

#include <ia20/commonlib/types.h>

namespace IA20 {

/*************************************************************************/
/** The Timestamp class.
 *
 */
class Timestamp {
public:

	Timestamp(bool bCurrentSystemDate=false);
	Timestamp(const Timestamp& other);
	Timestamp(const String& strValue);

	~Timestamp()throw();

	String toString()const;
	String toDateTimeString()const;
	String toTimeString()const;
	String toDateString()const;

	String toString(const String& strFormat)const;

	void fromString(const String& strValue);
	void fromDateTimeString(const String& strValue);
	void fromDateString(const String& strValue);
	void fromTimeString(const String& strValue);

	void fromString(const String& strValue, const String& strFormat);
	void fromString(const String& strValue, const String& strFormat, bool toLocalTime);

	void toPosixTime(time_t& aValue)const;
	void toPosixTime(struct tm& aValue)const;
	void fromPosixTime(time_t aValue);
	void fromPosixTime(struct tm& aValue);

	void setDate(short iYear, short iMonth, short iDay);
	void setTime(short iHour, short iMin,   short iSec, int iUSec = 0);

	void setDateTime(short iYear, short iMonth, short iDay,
					 short iHour, short iMin,   short iSec, int iUSec = 0);

	void getDate(short &iYear, short &iMonth, short &iDay)const;

	void getTime(short &iHour, short &iMin, short &iSec, int& iUsec)const;

	void getDateTime(short &iYear, short &iMonth, short &iDay,
			     	 short &iHour, short &iMin, short &iSec, int& iUsec)const;

	Timestamp& operator=(const Timestamp& other){ assign(other); return *this;}

	Timestamp& operator+=(const Timestamp& other);
	Timestamp& operator-=(const Timestamp& other);

	operator long () const{
		return theValue.tv_sec;
	}

	operator double () const{
			return (double)(theValue.tv_sec) + theValue.tv_usec*0.000001;
	}

	void readSystemTimestamp();


	friend Timestamp operator+(const Timestamp& left, const Timestamp& right);
	friend Timestamp operator-(const Timestamp& left, const Timestamp& right);

	friend bool operator==(const Timestamp& left, const Timestamp& right);
	friend bool operator!=(const Timestamp& left, const Timestamp& right);
	friend bool operator<(const Timestamp& left, const Timestamp& right);
	friend bool operator>(const Timestamp& left, const Timestamp& right);
	friend bool operator<=(const Timestamp& left, const Timestamp& right);
	friend bool operator>=(const Timestamp& left, const Timestamp& right);

protected:

	struct timeval theValue;

	void assign(const Timestamp& other);

private:

	short readNumber(std::istream& is, int iLimitDigits=0);
	short readUSec(std::istream& is, bool bOptionalDecimal);
	short readZoneOffset(std::istream& is);
	short readAbbreviatedMonth(std::istream& is);
	time_t gmtToLocal(struct tm* timeinfo);

	static const int CFPrec  = 3;
	static const int CFRound = 1000;

	static const int UFPrec  = 6;
};


/*************************************************************************/
}

#endif /* _IA20_Timestamp_H_ */

