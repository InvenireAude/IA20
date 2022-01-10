/*
 * File: Subscription.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Subscription_H_
#define _IA20_IOT_Subscription_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/StringRef.h>

#include "Connection.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Subscription class.
 *
 */
class Subscription {
public:

	typedef uint32_t HandleType;

	~Subscription() throw();
	Subscription(HandleType aHandle,
				 const Tools::StringRef& strTopic,
				 Connection::HandleType aConnectionHandle);

	inline void setNext(Subscription* pNext){
		IA20_LOG(true,"Next: "<<strTopic<<" => "<<(pNext ? pNext->strTopic : "null"));
		this->pNext = pNext;
	}

	inline Subscription* getNext()const{
		return pNext;
	}

	inline Connection::HandleType getConnectionHandle()const{
		return aConnectionHandle;
	}

	inline HandleType getHandle()const{
		return aHandle;
	}

protected:
	String         strTopic;//TODO
	Subscription*  pNext;
	HandleType     aHandle;

	Connection::HandleType aConnectionHandle;
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Subscription_H_ */
