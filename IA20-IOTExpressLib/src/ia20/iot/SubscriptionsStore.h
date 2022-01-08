/*
 * File: SubscriptionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_SubscriptionsStore_H_
#define _IA20_IOT_SubscriptionsStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/StringRef.h>

#include <list>
#include <unordered_map>

#include "Connection.h"
#include "Subscription.h"
#include "ActionsStore.h"

namespace IA20 {
namespace IOT {
namespace Tools {
	class WordsMap;
}
/*************************************************************************/
/** The SubscriptionsStore class.
 *
 */
class ConnectionsStore;
class TopicsStore;
class ActivityStore;

class SubscriptionsStore {

	typedef std::list< std::unique_ptr<Subscription> > SubscriptionList;

	typedef std::unordered_map<Connection::HandleType, 
		       std::unique_ptr<SubscriptionList> > SubscriptionsMap;

public:

	virtual ~SubscriptionsStore() throw();
	SubscriptionsStore(ConnectionsStore* pConnectionsStore, TopicsStore* pTopicsStore);


	void addSubscription(Connection::HandleType aHandle, 
						 const Tools::StringRef& strTopic, 
						 uint8_t iOptions);

	SubscriptionList* getList(Connection::HandleType aHandle);

	
protected:

	ConnectionsStore* pConnectionsStore;
	TopicsStore*      pTopicsStore;
	
	SubscriptionsMap hmSubscriptions;

	Subscription::HandleType iNextHandle;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_SubscriptionsStore_H_ */
