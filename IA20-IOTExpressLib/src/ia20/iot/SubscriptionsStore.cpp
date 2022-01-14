/*
 * File: SubscriptionsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "SubscriptionsStore.h"
#include <ia20/iot/tools/WordsMap.h>
#include <ia20/iot/tools/WordTokens.h>

#include "SubscriptionsStore.h"
#include "Subscription.h"
#include "TopicsStore.h"
#include "Topic.h"

#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
SubscriptionsStore::SubscriptionsStore(ConnectionsStore* pConnectionsStore, 
									  TopicsStore*      pTopicsStore):
	pConnectionsStore(pConnectionsStore),
	pTopicsStore(pTopicsStore),
	iNextHandle(0){
	IA20_TRACER;
}
/*************************************************************************/
SubscriptionsStore::~SubscriptionsStore() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void SubscriptionsStore::addSubscription(Connection::HandleType aConnectionHandle, 
										const Tools::StringRef& strTopic,  
										uint8_t iOptions){
	IA20_TRACER;
	
	SubscriptionsStore::SubscriptionList *pList = getList(aConnectionHandle);

	std::unique_ptr<Subscription> ptrSubscription(new Subscription(iNextHandle, strTopic, aConnectionHandle, iOptions));
	
	pTopicsStore->getOrCreateTopic(strTopic)->addSubscription(ptrSubscription.get());

	pList->push_back(ptrSubscription.get());

	hmSubscriptions[iNextHandle++] = std::move(ptrSubscription);
}
/*************************************************************************/
SubscriptionsStore::SubscriptionList* SubscriptionsStore::getList(Connection::HandleType aHandle){
	IA20_TRACER;

	SubscriptionsStore::SubscriptionList* pResult;

	ConnectionSubsMap::iterator it = hmConnectionSubs.find(aHandle);

	if(it == hmConnectionSubs.end()){
		hmConnectionSubs[aHandle].reset(new SubscriptionList);
		pResult = hmConnectionSubs[aHandle].get();
	} else {
		pResult = it->second.get();
	}

	return pResult;
}
/*************************************************************************/
Subscription* SubscriptionsStore::lookup(Subscription::HandleType aHandle)const{
	IA20_TRACER;

	SubscriptionsMap::const_iterator it = hmSubscriptions.find(aHandle);

	if(it == hmSubscriptions.end()){
		IA20_THROW(ItemNotFoundException("Subscription: ")<<(void*)(long)aHandle);
	} 

	return it->second.get();
}
/*************************************************************************/

}
}
