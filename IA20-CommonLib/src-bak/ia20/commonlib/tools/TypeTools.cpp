/*
 * File: IA20-CommonLib/src/commonlib/tools/TypeTools.cpp
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
#include "TypeTools.h"

#include "../logger/logger.h"
#include "../exception/DataException.h"
#include "../exception/BadUsageException.h"
#include "string.h"

#include <stdlib.h>
#include <errno.h>
#include <string>
#include <codecvt>
#include <locale>
#include <exception>

namespace IA20 {

/*************************************************************************/
int TypeTools::StringToInt(const std::string& strValue) {
	IA20_TRACER;

	int iResult;

#if __cplusplus >= 201103L

	try{
		iResult = std::stoi(strValue.c_str(),0,10);
	}catch(std::exception& e){
		IA20_THROW(BadUsageException("Conversion error: [" + strValue + "]"));
	}

#else

 	std::stringstream ss(strValue);
 	ss >> iResult;
 	if(ss.fail())
 		IA20_THROW(BadUsageException("Conversion error: [" + strValue + "]"));

#endif


	return iResult;

}
/*************************************************************************/
long TypeTools::StringToLong(const std::string& strValue) {
	IA20_TRACER;

	long lResult;

#if __cplusplus >= 201103L

	try{
		lResult = std::stoll(strValue.c_str(),0,10);
	}catch(std::exception& e){
		IA20_THROW(BadUsageException("Conversion error: [" + strValue + "]"));
	}

#else

 	std::stringstream ss(strValue);
 	ss >> iResult;
 	if(ss.fail())
 		IA20_THROW(BadUsageException("Conversion error: [" + strValue + "]"));

#endif


	return lResult;

}

/*************************************************************************/
double TypeTools::StringToFloat(const std::string& strValue) {
	std::stringstream ss(strValue);

	double fResult;
	ss >> fResult;

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "["<<strValue<<"]="<<fResult);

	return fResult;
}

/*************************************************************************/
double TypeTools::StringToDouble(const std::string& strValue) {
	std::stringstream ss(strValue);

	double fResult;
	ss >> fResult;

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "["<<strValue<<"]="<<fResult);

	return fResult;
}

/*************************************************************************/
bool TypeTools::StringToBoolean(const std::string& strValue, bool bAllowNumeric) {

	std::stringstream ss(strValue);

	bool bResult = false;

	if (strValue.compare("false") == 0) {
		bResult = false;
	} else if (strValue.compare("true") == 0) {
		bResult = true;
	} else {

		if (!bAllowNumeric)
			IA20_THROW(DataException(std::string("Bad boolean value:")+=strValue));
	}

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "["<<strValue<<"]="<<bResult);

	return bResult;
}

/*************************************************************************/
void TypeTools::IntToString(int iValue, std::string& strValue, int iWidth) {
	std::stringstream ss;
	if (iWidth > 0) {
		ss.width(iWidth);
		ss.fill('0');
	}
	ss << iValue;
	strValue = ss.str();
}
/*************************************************************************/
void TypeTools::LongToString(long lValue, std::string& strValue, int iWidth) {
	std::stringstream ss;
	if (iWidth > 0) {
		ss.width(iWidth);
		ss.fill('0');
	}
	ss << lValue;
  strValue = ss.str();
}
/*************************************************************************/
void TypeTools::FloatToString(float fValue, std::string& strValue, int iWidth) {
	std::stringstream ss;

	ss.setf(std::ios::fixed,std::ios::floatfield);
	ss.setf(std::ios::showpoint);

	if (iWidth > 0) {
		ss.width(iWidth);
		ss.fill('0');
	}
	ss << fValue;
	strValue = ss.str();
}
/*************************************************************************/
void TypeTools::DoubleToString(double fValue, std::string& strValue, int iWidth) {
	std::stringstream ss;

	ss.setf(std::ios::fixed,std::ios::floatfield);
	ss.setf(std::ios::showpoint);

	if (iWidth > 0) {
		ss.width(iWidth);
		ss.fill('0');
	}
	ss << fValue;
	strValue = ss.str();
}
/*************************************************************************/
void TypeTools::BoolToString(bool bValue, std::string& strValue) {

	strValue = bValue ? "true" : "false";
}

/*************************************************************************/
std::string TypeTools::FloatToString(double fValue, int iWidth) {
	std::string strValue;
	FloatToString(fValue, strValue, iWidth);
	return strValue;
}

/*************************************************************************/
std::string TypeTools::BoolToString(bool bValue) {
	std::string strValue;
	BoolToString(bValue, strValue);
	return strValue;
}
/*************************************************************************/
std::string TypeTools::IntToString(int iValue, int iWidth) {
	std::string strValue;
	IntToString(iValue, strValue, iWidth);
	return strValue;
}
/*************************************************************************/
std::string TypeTools::LongToString(long lValue, int iWidth) {
	std::string strValue;
	LongToString(lValue, strValue, iWidth);
	return strValue;
}
/*************************************************************************/
std::string TypeTools::CharToString(char cValue) {
	IA20_TRACER;
	std::string strResult;
	strResult = cValue;
	return strResult;
}
/*************************************************************************/
void TypeTools::Tokenize(const std::string& strInput, StringList& refOutput, char cDelimiters) {
	IA20_TRACER;

	size_t iStart = 0;
	size_t iCursor = 0;

	refOutput.clear();
	while (iCursor != std::string::npos) {

		iCursor = strInput.find(cDelimiters, iStart);

		if (iCursor == std::string::npos) {
			refOutput.push_back(strInput.substr(iStart, std::string::npos));
			IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "["<<strInput.substr(iStart, std::string::npos)<<"]");
		} else {
			refOutput.push_back(strInput.substr(iStart, iCursor - iStart));
			IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "["<<strInput.substr(iStart, iCursor - iStart)<<"]");
			iStart = iCursor + 1;
		}

	}
}
/*************************************************************************/
void TypeTools::ChopArguments(const std::string& strArgument, std::string& strName, std::string& strValue, char cDelimter ){
	IA20_TRACER;

	bool bName=true;


	strName.clear();
	strValue.clear();

	strName.reserve(strArgument.length());
	strValue.reserve(strArgument.length());

	for(std::string::const_iterator it=strArgument.begin(); it!=strArgument.end(); it++){
		if(*it == cDelimter && bName){
			bName=false;
		}else{
			if(bName)
				strName+=*it;
			else
				strValue+=*it;
		}
	}

	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"Argument["<<strName<<"]=["<<strValue<<"]");
}
/*************************************************************************/
std::string TypeTools::Replace(const std::string& strInput, const std::string& strPattern, const std::string& strNewText){
	IA20_TRACER;

	std::size_t iPos=0;
	std::size_t iLastPos=iPos;

	std::string strResult;
	std::size_t iPatternLen=strPattern.length();

	while( (iPos=strInput.find(strPattern,iPos)) != std::string::npos){
		strResult+=strInput.substr(iLastPos,iPos - iLastPos);
		strResult+=strNewText;
		iPos+=iPatternLen;
		iLastPos=iPos;
	};

	strResult+=strInput.substr(iLastPos);
	return strResult;
}

/*************************************************************************/
static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t,
	   	AllocatorFactory< wchar_t, &IA20::MemoryManager::GetAllocator >,
	   	AllocatorFactory< char, &IA20::MemoryManager::GetAllocator >
     > _strconverter;
/*************************************************************************/
std::string TypeTools::FromWString(const WString& strValue){
 return _strconverter.to_bytes(strValue);
}
/*************************************************************************/
WString TypeTools::ToWString(const std::string& strValue){
return _strconverter.from_bytes(strValue);
}
/*************************************************************************/
}
