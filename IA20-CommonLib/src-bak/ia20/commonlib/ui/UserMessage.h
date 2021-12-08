/*
 * File: IA20-CommonLib/src/commonlib/ui/UserMessage.h
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

#ifndef _IA20_UserMessage_H_
#define _IA20_UserMessage_H_

/*************************************************************************/
#include <vector>

#include "MessageCatalog.h"

//TODO (M) message subscribers classes (stderr,syslog file etc) other logs, too.

namespace IA20{

/** The class. */
 class UserMessage{

public:
	UserMessage(MessageCatalog::MsgId id);

	virtual ~UserMessage();

	 UserMessage & operator<<(const char *s);
	 UserMessage & operator<<(const std::string& s);
	 UserMessage & operator<<(long  iValue);
	 UserMessage & operator<<(void* pValue);

protected:

	MessageCatalog::MsgId id;

	std::vector<std::string> tabValues;

	void show(const std::string& strText);



 };

}
/* namespaces IA20 */

#endif /*DISPLAYABLEMESSAGE_H_*/
