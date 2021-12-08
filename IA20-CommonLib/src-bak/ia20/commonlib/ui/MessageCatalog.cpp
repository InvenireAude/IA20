/*
 * File: IA20-CommonLib/src/commonlib/ui/MessageCatalog.cpp
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#include "MessageCatalog.h"
#include "../commonlib.h"
#include "commonlib/tools/EnvTools.h"
#include "commonlib/sys/fs/DirectoryReader.h"

#include "../threads/Mutex.h"

/*************************************************************************/
namespace IA20{

const char MessageCatalog::SPECIAL_DSPMSG_CHAR = '&';

/*************************************************************************/
MessageCatalog::MessageCatalog(){
	IA20_TRACER;

}
/*************************************************************************/
MessageCatalog::~MessageCatalog(){
	IA20_TRACER;

}
/*************************************************************************/
void MessageCatalog::init(){
	IA20_TRACER;
}
/*************************************************************************/
void MessageCatalog::loadFile(const std::string& strFileName){
	IA20_TRACER;

	IA20_LOG(IA20::LogLevel::INSTANCE.isSystem(),"Loading Messages from:["<<strFileName<<"]");


	InputFile ifMessages(strFileName);
	ifMessages.open();

	if(! ifMessages.isOpenedAndGood() ){
		IA20_THROW(ConfigException(strFileName));
	}

	int iLineNo=0;

	while(ifMessages.isOpenedAndGood()){

		char sLine[256];
		ifMessages.getStreamRef().getline(sLine, 255, '\n');

			iLineNo++;

			if(*sLine == '#')
				continue;

			std::string strLine(sLine);
			int iPos = strLine.find('=');

			if(iPos == std::string::npos){
				IA20_THROW(ConfigException("Missing '='",strFileName.c_str(),iLineNo));
			}

			int iIdx=TypeTools::StringToInt(strLine.substr(0,iPos));
			std::string strMessage=strLine.substr(iPos+1);

			IA20_LOG(IA20::LogLevel::INSTANCE.isDetailedInfo(),"["<<iIdx<<"]=["<<strMessage<<"]");

			hmMessages[iIdx]   		= strMessage;

			return;
		}

}
/*************************************************************************/
void MessageCatalog::MessageCatalogUpdater::UpdateCatalog(MsgId idMsg, const std::string& strMessage){
	MessageCatalog::GetInstance()->update(idMsg, strMessage);
}
/*************************************************************************/
static Mutex _mutex;
/*************************************************************************/
void MessageCatalog::update(MsgId idMsg, const std::string& strMessage){

	Mutex::Locker locker(_mutex);

	if(hmMessages.count(idMsg)){
		IA20_LOG(LogLevel::INSTANCE.isError(),"Two User Messages with the same id: "<<idMsg<<", has: "<<hmMessages[idMsg]<<", got: "<<strMessage);
		return;
	}

	hmMessages[idMsg] = strMessage;
}
/*************************************************************************/
void MessageCatalog::fillMessageText(std::string& strOutput,
					 				 MsgId   iMessageId,
									 const std::vector<std::string>& tabValues){
	IA20_TRACER;

	std::string strMsg;

	Mutex::Locker locker(_mutex);

	if(hmMessages.count(iMessageId) == 0){
		std::stringstream ss;
		ss<<"IA20E-0000 No Text for message with id="<<iMessageId;
		strMsg=ss.str();
	}else{
		 strMsg=hmMessages[iMessageId];
	}


	std::string::iterator it;
	char c;

	for(it=strMsg.begin(); it != strMsg.end(); it++){
		c=*it;
		if(c != SPECIAL_DSPMSG_CHAR)
			strOutput+=c;
		else{
			if(it != strMsg.end()){
				c=*(++it);
				switch(c){
					case '1': if(tabValues.size() >= 1) strOutput+=tabValues[0]; break;
					case '2': if(tabValues.size() >= 2) strOutput+=tabValues[1]; break;
					case '3': if(tabValues.size() >= 3) strOutput+=tabValues[2]; break;
					case '4': if(tabValues.size() >= 4) strOutput+=tabValues[3]; break;
          case '5': if(tabValues.size() >= 5) strOutput+=tabValues[4]; break;
				  default:
					  strOutput+=SPECIAL_DSPMSG_CHAR;
					  strOutput+=c;
				}/* switch */
			}else
				strOutput+=SPECIAL_DSPMSG_CHAR;
		}/* IF: '&_' */

	}/* FOR: */

}
/*************************************************************************/
const std::string MessageCatalog::ENV_DSP_MSGS_DIR("IA20_DSP_MSGS_DIR");
const std::string MessageCatalog::ENV_DSP_MSGS_DIR_DEFALUT("msgs");
/*************************************************************************/
void MessageCatalog::loadExternalMessages(){
	IA20_TRACER;

	Mutex::Locker locker(_mutex);

	std::string strDirectory(ENV_DSP_MSGS_DIR_DEFALUT);
	EnvTools::GetEnv(ENV_DSP_MSGS_DIR,strDirectory);

	IA20_DFT_FACTORY<SYS::FS::DirectoryReader>::PtrHolder ptrDirectoryReader;

	ptrDirectoryReader=IA20_DFT_FACTORY<SYS::FS::DirectoryReader>::Create(strDirectory);
	ptrDirectoryReader->read();

for (SYS::FS::DirectoryReader::const_iterator it = ptrDirectoryReader->begin(); it != ptrDirectoryReader->end(); it++) {
		std::string strFile((*it)->strFile);
		//TODO (LL) match sufix.
		loadFile(strFile);
	}
}
/*************************************************************************/
}/* namespace IA20 */

