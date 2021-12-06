/*
 * File: IA20-CommonLib/src/commonlib/streams/InputFileTail.cpp
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
#include "InputFileTail.h"

#include "ia20/commonlib/exception/InternalException.h"
#include "ia20/commonlib/exception/ItemNotFoundException.h"
#include "ia20/commonlib/exception/EndOfDataException.h"

#include "../memory/memory.h"

#include <ia20/commonlib/tools/MiscTools.h>

#include <iostream>
#include <fstream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <ia20/commonlib/threads/Thread.h>

namespace IA20 {

/*************************************************************************/
InputFileTail::InputFileTail(const String& strName,
							 bool bFollowName,
							 int iPoolInterval):
		InputFile(strName),
		bFollowName(bFollowName),
		iPoolInterval(iPoolInterval),
		iLastInode(0){

	IA20_TRACER;

	struct stat statData;

	this->bFollowName=true;

}
/*************************************************************************/
InputFileTail::~InputFileTail() {
	IA20_TRACER;
	close();
}
/*************************************************************************/
String InputFileTail::nextLine(int iTimeoutMS){
	IA20_TRACER;

	String strResult;

	struct stat statData;

	while(true) {

		IA20_LOG(IA20::LogLevel::INSTANCE.isDetailedInfo(),"tail, timeout: "<<iTimeoutMS);

		if(!is.is_open())
			try{

				IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"opening: "<<strName);
				open();

				if(stat(strName.c_str(), &statData) == -1)
					IA20_THROW(SystemException(strName));

				iLastInode=statData.st_ino;
				IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"inode: "<<iLastInode);

			}catch(SystemException& e){
				IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"waiting for file: "<<strName);
			}


		if(is.is_open()){

				std::getline(is,strResult);

				if(!is.eof())
					return strResult;

				is.clear();

				if(bFollowName) {

					IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"following: "<<strName);

					if(stat(strName.c_str(), &statData) == -1){
						if(errno == ENOENT){
							this->close();
						}else
							IA20_THROW(SystemException(strName));
					}


					if(iLastInode != statData.st_ino) {
						this->close();
						IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(),"["<<strName<<"] recycle.");
						continue;
					}

				}
		}


		if(iTimeoutMS == 0)
			IA20_THROW(EndOfDataException(strName));

		Thread::Cancellation tc(true);

		if(CInfiniteWait == iTimeoutMS || iPoolInterval < iTimeoutMS) {
			usleep(iPoolInterval*1000);
			if(iTimeoutMS != iTimeoutMS)
				iTimeoutMS-=iPoolInterval;
		} else {
			usleep(iTimeoutMS*1000);
			iTimeoutMS=0;
		}

	}



}
/*************************************************************************/
}
