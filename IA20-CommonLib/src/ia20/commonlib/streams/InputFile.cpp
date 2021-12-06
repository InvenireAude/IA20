/*
 * File: IA20-CommonLib/src/commonlib/streams/InputFile.cpp
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
#include "InputFile.h"

#include "ia20/commonlib/exception/InternalException.h"
#include "ia20/commonlib/exception/ItemNotFoundException.h"

#include "../memory/memory.h"

#include <ia20/commonlib/tools/MiscTools.h>

#include <iostream>
#include <fstream>
#include <string>

namespace IA20 {

/*************************************************************************/
InputFile::InputFile(const String& strName):File(strName){
	IA20_TRACER;

	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"Create InputFile for:["<<strName<<"]");

}
/*************************************************************************/
InputFile::~InputFile() {
	IA20_TRACER;
	close();
}

/*************************************************************************/
void InputFile::open() {
	IA20_TRACER;
	IA20_LOG(IA20::LogLevel::INSTANCE.isSystem(),"["<<strName<<"]");
	is.open(strName.c_str());

	//TODO (L) system exception is it used somehwere ?

	if(!isOpenedAndGood())
		IA20_THROW(SystemException(strName));

	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"Is good !");
}

/*************************************************************************/
void InputFile::close() {
	IA20_TRACER;
	is.close();
}

/*************************************************************************/
std::istream& InputFile::getStreamRef(){
	IA20_TRACER;
	if(!is.good()){
		IA20_THROW(InternalException(String("File not opened: ")+=strName));
	}
	return is;
}

/*************************************************************************/
bool InputFile::isOpenedAndGood(){
	IA20_TRACER;
	return is.good() && is.is_open();
}
/*************************************************************************/
void InputFile::LoadString(const String& strFileName, String& strValue){
	IA20_TRACER;

	InputFile file(strFileName);
	file.open();

	StringStream ssTmp;

	MiscTools::CopyStreams(file.getStreamRef(),ssTmp);

	strValue=ssTmp.str();

}
/*************************************************************************/
void InputFile::LoadStdin(String& strValue){
	IA20_TRACER;

	StringStream ssInput;
	const int C_BlockSize=4096;

	char sBuffer[C_BlockSize+1];

	while(std::cin.good()){
		std::cin.read(sBuffer,C_BlockSize);
		sBuffer[std::cin.gcount()]=0;
		ssInput<<sBuffer;
	}

	strValue=ssInput.str();
}
/*************************************************************************/
}
