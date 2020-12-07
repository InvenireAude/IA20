/*
 * File: IA20-CommonLib/src/commonlib/ui/UserMessage.cpp
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

#include "UserMessage.h"
#include "../commonlib.h"
#include "../time/Timestamp.h"
#include <unistd.h>
#include <iostream>
#include "../logger/Logger.h"

namespace IA20{

/*************************************************************************/
UserMessage::UserMessage(MessageCatalog::MsgId id):id(id){
	IA20_TRACER;
}
/*************************************************************************/
UserMessage::~UserMessage(){
	IA20_TRACER;
	String strText;
	MessageCatalog::GetInstance()->fillMessageText(strText,id,tabValues);
	show(strText);
}
/*************************************************************************/
UserMessage& UserMessage::operator<<(const char *s) {
    tabValues.push_back(s);
    return *this;
}
/*************************************************************************/
UserMessage& UserMessage::operator<<(const String& s) {
	tabValues.push_back(s);
    return *this;
}
/*************************************************************************/
UserMessage& UserMessage::operator<<(long iValue) {
	tabValues.push_back(TypeTools::IntToString(iValue));
    return *this;
}
/*************************************************************************/
UserMessage& UserMessage::operator<<(void* pValue) {
	StringStream ss;
	ss<<pValue;
	tabValues.push_back(ss.str());
    return *this;
}
/*************************************************************************/
void UserMessage::show(const String& strText){
	IA20_TRACER;
	// TODO (M) syslog

	static Mutex mutex;

	Mutex::Locker locker(mutex);

	if(LogLevel::INSTANCE.isDspMsg())
		Logger::GetInstance()->addEntry(strText.c_str());

}

}
