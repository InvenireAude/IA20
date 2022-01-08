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

	std::unique_ptr<Subscription> ptrSubscription(new Subscription(iNextHandle++, strTopic, aConnectionHandle));
	
	pTopicsStore->getTopic(strTopic)->addSubscription(ptrSubscription.get());

	pList->push_back(std::move(ptrSubscription));

}
/*************************************************************************/
SubscriptionsStore::SubscriptionList* SubscriptionsStore::getList(Connection::HandleType aHandle){
	IA20_TRACER;

	SubscriptionsStore::SubscriptionList* pResult;

	SubscriptionsMap::iterator it = hmSubscriptions.find(aHandle);

	if(it == hmSubscriptions.end()){
		hmSubscriptions[aHandle].reset(new SubscriptionList);
		pResult = hmSubscriptions[aHandle].get();
	} else {
		pResult = it->second.get();
	}

	return pResult;
}
/*************************************************************************/

}
}
