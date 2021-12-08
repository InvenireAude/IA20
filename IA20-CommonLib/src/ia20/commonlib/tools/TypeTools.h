/*
 * File: IA20-CommonLib/src/commonlib/tools/TypeTools.h
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
#ifndef TYPETOOLS_H_
#define TYPETOOLS_H_

#include "../types.h"
#include <time.h>
#include <sys/time.h>

namespace IA20 {

/** The class. */
 class TypeTools{
public:

	 inline static unsigned int GetTimeMS(){
	     struct timeval tv;
	     ::gettimeofday(&tv, NULL);
	 	return tv.tv_sec*1000 +  tv.tv_usec/1000;
	 }

	static int    StringToInt(const String& strValue);
  static Long   StringToLong(const String& strValue);
	static Float  StringToFloat(const String& strValue);
	static bool   StringToBoolean(const String& strValue, bool bAllowNumeric = false);

	static void IntToString(int         iValue, String& strValue, int iWidth = -1);
  static void LongToString(Long       lValue, String& strValue, int iWidth = -1);
	static void FloatToString(Float     fValue, String& strValue, int iWidth = -1);

	static void BoolToString(bool bValue, String& strValue);

	static String IntToString(int iValue, int iWidth = -1);
  static String LongToString(Long lValue, int iWidth = -1);
	static String FloatToString(Float     fValue, int iWidth = -1);
	static String BoolToString(bool bValue);


	static String CharToString(char cValue);

	static void ChopArguments(const String& strArgument, String& strName, String& strValue, char cDelimter = '=');

	static void Tokenize(const String& strInput, StringList& refOutput, char cDelimiter = '/');

	static String Replace(const String& strInput, const String& strPattern, const String& strNewText);

  static String  FromWString(const WString& strValue);
  static WString ToWString(const String& strValue);

};

}
#endif /* TYPETOOLS_H_ */
