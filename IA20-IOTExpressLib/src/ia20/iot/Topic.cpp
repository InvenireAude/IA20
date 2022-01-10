/*
 * File: Topic.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Topic.h"

#include "Subscription.h"

#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
// Topic::Topic(){
// 	IA20_TRACER;
// }

// /*************************************************************************/
// Topic::~Topic() throw(){
// 	IA20_TRACER;
// }
/*************************************************************************/
void Topic::addSubscription(Subscription* pSubscription){
	pSubscription->setNext(pFirst);
	pFirst = pSubscription;
}
/*************************************************************************/
}
}
