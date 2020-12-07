/*
 * File: IA20-CommonLib/src/commonlib/tools/EnvTools.cpp
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
#include "EnvTools.h"

#include <ia20/commonlib/commonlib.h>

#include <stdlib.h>

namespace IA20 {


/*************************************************************************/
bool EnvTools::GetEnv(const String& strName, String& strValue){
	IA20_TRACER;

	const char* sValue = getenv(strName.c_str());

	if(sValue == NULL)
		return false;

	strValue=sValue;
	return true;
}
/*************************************************************************/
bool EnvTools::GetEnv(const String& strName, int& iValue){
	IA20_TRACER;

	const char* sValue = getenv(strName.c_str());

	if(sValue == NULL)
		return false;

  String strValue(sValue);
	iValue = TypeTools::StringToInt(strValue);

	return true;
}
/*************************************************************************/
bool EnvTools::GetBooleanEnv(const String& strName){
	IA20_TRACER;

	const char* sValue = getenv(strName.c_str());

	if(sValue == NULL)
		return false;

	return *sValue == 'Y' || *sValue == 'y';
}
/*************************************************************************/
String EnvTools::GetEnv(const String& strName){
	IA20_TRACER;
	String strValue;

	if(!GetEnv(strName,strValue))
		IA20_THROW(ItemNotFoundException(strName));

	return strValue;
}
/*************************************************************************/
String EnvTools::GetEnvWithDefault(const String& strName, const String& strDefault){
	IA20_TRACER;
	String strValue;

	if(!GetEnv(strName,strValue))
    return strDefault;

	return strValue;
}
/*************************************************************************/
void EnvTools::GetEnvTokenized(const String& strName,
							   StringList& lstValues,
							   char cDelimiter ){
	IA20_TRACER;

	String strValue;
	if(GetEnv(strName,strValue))
		TypeTools::Tokenize(strValue, lstValues, cDelimiter);
}
/*************************************************************************/
String EnvTools::Substitute(const String& strPattern, bool bFailIfMissing){
	IA20_TRACER;

	StringStream ssResult;

	for(String::const_iterator it=strPattern.begin(); it != strPattern.end(); it++){

		if(*it == '$'){

			if(++it != strPattern.end() && *it == '{'){

				String strEnv;

				while(++it != strPattern.end() && *it != '}')
					strEnv+=(*it);

				if(*it != '}')
					IA20_THROW(BadUsageException("Missing '}' in environment pattern."))

				String strValue;

				if(GetEnv(strEnv,strValue)){
					ssResult<<strValue;
				}else{

					if(bFailIfMissing)
						IA20_THROW(ItemNotFoundException(strEnv));

					ssResult<<"${"<<strEnv<<"}";
				}

			}
		}else{
			ssResult<<*it;
		}


	}

	return ssResult.str();
}
/*************************************************************************/
}
