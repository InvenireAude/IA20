/*
 * File: IA20-CommonLib/src/commonlib/ui/MessageCatalog.h
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

#ifndef _IA20_MessageCatalog_H_
#define _IA20_MessageCatalog_H_

#include "../memory/InstanceFeature.h"
#include "../types.h"

#include<map>
#include<vector>
/*************************************************************************/

namespace IA20{

/** The class. */
 class MessageCatalog : public InstanceFeature<MessageCatalog>{

public:

	typedef unsigned int MsgId;

	MessageCatalog();
	virtual ~MessageCatalog();

	void fillMessageText(String& strOutput,
						 MsgId     iMessageId,
					     const std::vector<String>& tabValues);


	class MessageCatalogUpdater {
		protected:
		static void UpdateCatalog(MsgId, const String& strMessage);
	};

protected:

	typedef std::map<int,String> DisplayableMessageMap;

	void init();

	DisplayableMessageMap hmMessages;

	void update(MsgId idMsg, const String& strMessage);

	static const String ENV_DSP_MSGS_DIR;
	static const String ENV_DSP_MSGS_DIR_DEFALUT;
	static const char   SPECIAL_DSPMSG_CHAR;
};


}/* namespace IA20 */

#endif /*_IA20_MessageCatalog_H_*/
