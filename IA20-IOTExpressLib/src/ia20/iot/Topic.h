/*
 * File: Topic.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Topic_H_
#define _IA20_IOT_Topic_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/WordsMap.h>

#include <ia20/iot/logger/LogLevel.h>
#include "Message.h"

namespace IA20 {
namespace IOT {
class Subscription;

/*************************************************************************/
/** The Topic class.
 *
 */
class Topic {
public:

	//~Topic() throw();

	inline Topic(Tools::WordsMap::WordIdType iToken, Topic* pParent, const String& strTokenName):
	iToken(iToken),
    pParent(pParent),
    aRetainedMessageHandle(Message::CNullHandle),
	pFirst(NULL){
		if(pParent){
			strName = pParent->getName() + "/";
		}
		strName += strTokenName;
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsDetailedInfo,"New token: "<<(int)iToken<<", name: "<<strName);
	};

	inline Tools::WordsMap::WordIdType getToken()const{
		return iToken;
	}

	void addSubscription(Subscription* pSubscription);

	inline Subscription* getFirstSubscription()const{
		return pFirst;
	}

	inline bool hasFirstSubscription()const{
		return pFirst != NULL;
	}

	inline void setRetained(Message::HandleType aRetainedMessageHandle = Message::CNullHandle){
		this->aRetainedMessageHandle = aRetainedMessageHandle;
	}

	inline bool hasRetained()const{
		return aRetainedMessageHandle != Message::CNullHandle;
	}

	inline Message::HandleType getRetained()const{
		return aRetainedMessageHandle;
	}
	
	class Callback {
		public:
		virtual void onSubscription(const Subscription* pSubscription)=0;
	};

	class RetainCallback {
		public:
		virtual void onTopic(const Topic* pTopic)=0;
	};

	void iterate(Callback* pCallback);

	inline Topic* getParent()const{
		return pParent;
	}

	inline const String& getName()const{
		return strName;
	}

protected:
	Tools::WordsMap::WordIdType iToken;
  	Topic* pParent;
	Subscription* pFirst;
	Message::HandleType aRetainedMessageHandle;

	String strName;//TODO
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Topic_H_ */
