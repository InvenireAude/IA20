/*
 * File: Subscription.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Subscription.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
Subscription::Subscription(HandleType aHandle, 
	const Tools::StringRef& strTopic,
	Connection::HandleType aConnectionHandle):
	strTopic(strTopic),
	aHandle(aHandle),
	aConnectionHandle(aConnectionHandle),
	pNext(NULL){
	IA20_TRACER;
	
	IA20_LOG(true,"Subscription: ["<<this->strTopic<<"]:"<<(void*)(long)aHandle);

}
/*************************************************************************/
Subscription::~Subscription() throw(){
	IA20_TRACER;

	IA20_LOG(true,"~~~Subscription: ["<<this->strTopic<<"]:"<<(void*)(long)aHandle);
}
/*************************************************************************/
}
}
