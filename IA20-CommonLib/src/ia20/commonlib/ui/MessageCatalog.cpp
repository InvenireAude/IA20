/*
 * File: IA20-CommonLib/src/commonlib/ui/MessageCatalog.cpp
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#include "MessageCatalog.h"
#include "../commonlib.h"
#include <ia20/commonlib/tools/EnvTools.h>
//#include <ia20/commonlib/sys/fs/DirectoryReader.h>

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
void MessageCatalog::MessageCatalogUpdater::UpdateCatalog(MsgId idMsg, const String& strMessage){
	MessageCatalog::GetInstance()->update(idMsg, strMessage);
}
/*************************************************************************/
static Mutex _mutex;
/*************************************************************************/
void MessageCatalog::update(MsgId idMsg, const String& strMessage){

	Mutex::Locker locker(_mutex);

	if(hmMessages.count(idMsg)){
		IA20_LOG(LogLevel::INSTANCE.isError(),"Two User Messages with the same id: "<<idMsg<<", has: "<<hmMessages[idMsg]<<", got: "<<strMessage);
		return;
	}

	hmMessages[idMsg] = strMessage;
}
/*************************************************************************/
void MessageCatalog::fillMessageText(String& strOutput,
					 				 MsgId   iMessageId,
									 const std::vector<String>& tabValues){
	IA20_TRACER;

	String strMsg;

	Mutex::Locker locker(_mutex);

	if(hmMessages.count(iMessageId) == 0){
		StringStream ss;
		ss<<"IA20E-0000 No Text for message with id="<<iMessageId;
		strMsg=ss.str();
	}else{
		 strMsg=hmMessages[iMessageId];
	}


	String::iterator it;
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
}/* namespace IA20 */

