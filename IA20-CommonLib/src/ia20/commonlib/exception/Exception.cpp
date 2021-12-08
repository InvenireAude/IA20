/*
 * File: IA20-CommonLib/src/commonlib/exception/Exception.cpp
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
/* ChangeLog:
 *
 */

#include "Exception.h"

#include "../memory/memory.h"
#include "../logger/logger.h"
#include "../tools/TypeTools.h"

namespace IA20{

/*************************************************************************/
Exception::Exception(){
	IA20_TRACER;

	if(::IA20::LogLevel::INSTANCE.isStackTrace())
		appendStackTrace();
}
/*************************************************************************/
Exception::Exception(const String& strInfo){
	IA20_TRACER;

	if(::IA20::LogLevel::INSTANCE.isStackTrace())
		appendStackTrace();

	setInfo(strInfo);
}
/*************************************************************************/
Exception::~Exception() throw(){
	IA20_TRACER;
}

/*************************************************************************/
void Exception::setInfo(const char *sInfo){
	IA20_TRACER;
	IA20_CHECK_IF_NULL(sInfo);
	this->strInfo=sInfo;
}

/*************************************************************************/
void Exception::setInfo(const String& strInfo){
	IA20_TRACER;
	this->strInfo=strInfo;
}
/*************************************************************************/
const char* Exception::getName(){
	IA20_TRACER;
	return "Exception";
}

/*************************************************************************/
void Exception::appendStackTrace(){
	IA20_TRACER;

	StringStream ssStack;
#ifdef __GLIBC__
	IA20::PrintTrace(ssStack);
#else
	IA20_MY_STACK().printStack(ssStack);
#endif

	this->strStackTrace=ssStack.str();

}

/*************************************************************************/
String Exception::toString(){
	IA20_TRACER;

	StringStream ssInfo;
	printToStream(ssInfo);
	return ssInfo.str();
}

/*************************************************************************/
Exception& Exception::operator<< (const String& strInfo){
	IA20_TRACER;
	this->strInfo+=strInfo;
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (const char* sValue){
	IA20_TRACER;
	this->strInfo+=sValue;
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (int iValue){
	IA20_TRACER;
	strInfo+=TypeTools::IntToString(iValue);
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (long iValue){
	IA20_TRACER;
	strInfo+=TypeTools::IntToString(iValue);
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (unsigned long iValue){
	IA20_TRACER;
	strInfo+=TypeTools::IntToString(iValue);
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (Float fValue){
	IA20_TRACER;
	strInfo+=TypeTools::FloatToString(fValue);
	return *this;
}
/*************************************************************************/
Exception& Exception::operator<< (void* ptrValue){
	IA20_TRACER;
	StringStream ssTmp;
	ssTmp<<ptrValue;
	strInfo+=ssTmp.str();
	return *this;
}
/*************************************************************************/
const String& Exception::getInfo(){
	IA20_TRACER;
	if(LogLevel::INSTANCE.isInfo())
			this->strInfo += "\n" + this->strStackTrace;

	return this->strInfo;
}
/*************************************************************************/
void Exception::printToStream(std::ostream& os){
	IA20_TRACER;

	os<<" Exception '" <<getName()<<"' :"<<strInfo;
	if(!(strStackTrace.empty()))
		os<<"\n"<<strStackTrace<<"\n";

}


}/* namespace IA20 */
