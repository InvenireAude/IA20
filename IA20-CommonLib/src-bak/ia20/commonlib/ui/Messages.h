/*
 * File: IA20-CommonLib/src/commonlib/ui/Messages.h
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

#ifndef IA20_MESSAGES_H_
#define IA20_MESSAGES_H_

#include "MessageCatalog.h"

/*************************************************************************/

namespace IA20{

/** The class. */
 class Messages : public MessageCatalog::MessageCatalogUpdater{

public:

	static const MessageCatalog::MsgId	MSGI_Start             = 10001;
	static const MessageCatalog::MsgId	MSGI_Stop              = 10002;
	static const MessageCatalog::MsgId	MSGI_ConfigInfo        = 10003;
	static const MessageCatalog::MsgId	MSGE_ConfigError       = 20001;
	static const MessageCatalog::MsgId	MSGE_ThreadException   = 20101;
	static const MessageCatalog::MsgId	MSGE_InternalError     = 99999;

	Messages();
	virtual ~Messages();

protected:

	static Messages TheInstance;
};


}/* namespace IA20 */

#endif /*MESSAGECATALOG_H_*/
