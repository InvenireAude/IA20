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
#ifndef _IA20_TypeTools_H_
#define _IA20_TypeTools_H_


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

	static int    StringToInt(const std::string& strValue);
  static Long   StringToLong(const std::string& strValue);
	static double  StringToFloat(const std::string& strValue);
	static bool   StringToBoolean(const std::string& strValue, bool bAllowNumeric = false);

	static void IntToString(int         iValue, std::string& strValue, int iWidth = -1);
  static void LongToString(Long       lValue, std::string& strValue, int iWidth = -1);
	static void FloatToString(Float     fValue, std::string& strValue, int iWidth = -1);

	static void BoolToString(bool bValue, std::string& strValue);

	static std::string IntToString(int iValue, int iWidth = -1);
  static std::string LongToString(Long lValue, int iWidth = -1);
	static std::string FloatToString(Float fValue, int iWidth = -1);
	static std::string BoolToString(bool bValue);


	static std::string CharToString(char cValue);

	static void ChopArguments(const std::string& strArgument, std::string& strName, std::string& strValue, char cDelimter = '=');

	static void Tokenize(const std::string& strInput, StringList& refOutput, char cDelimiter = '/');

	static std::string Replace(const std::string& strInput, const std::string& strPattern, const std::string& strNewText);

  static std::string  FromWString(const WString& strValue);
  static WString ToWString(const std::string& strValue);

};

}
#endif /* TYPETOOLS_H_ */
