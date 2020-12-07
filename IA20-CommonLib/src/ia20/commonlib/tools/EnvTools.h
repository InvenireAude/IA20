/*
 * File: IA20-CommonLib/src/commonlib/tools/EnvTools.h
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
#ifndef _IA20_EnvTools_H_
#define _IA20_EnvTools_H_

#include "../types.h"

namespace IA20 {

/*************************************************************************/
/** The EnvTools class.
 *
 */
class EnvTools {
public:

	static bool GetEnv(const String& strName, String& strValue);
  static bool GetEnv(const String& strName, int& iValue);
	static bool GetBooleanEnv(const String& strName);

	static String GetEnv(const String& strName);
  static String GetEnvWithDefault(const String& strName,const String& strDefault);

	static void GetEnvTokenized(const String& strName, StringList& lstValues, char cDelimiter = ':');

	static String Substitute(const String& strPattern, bool bFailIfMissing = true);
};

/*************************************************************************/
}

#endif /* _IA20_EnvTools_H_ */
