/*
 * File: IA20-CommonLib/src/commonlib/streams/OutputFile.cpp
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
#include "OutputFile.h"
#include "../exception/InternalException.h"
#include "ia20/commonlib/exception/ItemNotFoundException.h"

#include "../memory/memory.h"

namespace IA20 {

/*************************************************************************/
OutputFile::OutputFile(const String& strName, bool bAppend):
  File(strName),
  bAppend(bAppend){
	IA20_TRACER;
	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"Create OutputFile for:["<<strName<<"]");
}

/*************************************************************************/
OutputFile::~OutputFile() {
	IA20_TRACER;
	close();
}

/*************************************************************************/
void OutputFile::open() {
	IA20_TRACER;

	os.open(strName.c_str(), bAppend ?  std::ofstream::app | std::ofstream::out : std::ofstream::out );

	if(!isOpenedAndGood())
			IA20_THROW(SystemException(strName));

	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),strName<<" - Is good !");

}
/*************************************************************************/
void OutputFile::close() {
	IA20_TRACER;
	os.close();
}

/*************************************************************************/
std::ofstream& OutputFile::getStreamRef(){
	IA20_TRACER;
	if(!os.good()){
		IA20_THROW(InternalException("file not opened."));
	}
	return os;
}

/*************************************************************************/
bool OutputFile::isOpenedAndGood(){
	IA20_TRACER;
	return os.good();
}

/*************************************************************************/
void OutputFile::SaveString(const String& strFileName, const String& strValue, bool bAppend){
	IA20_TRACER;

	OutputFile of(strFileName,bAppend);
	of.open();

	if(of.isOpenedAndGood())
		of.getStreamRef()<<strValue;

	of.close();
}
/*************************************************************************/
}
