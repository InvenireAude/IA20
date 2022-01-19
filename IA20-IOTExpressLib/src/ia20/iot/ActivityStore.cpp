/*
 * File: ActivityStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActivityStore.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
ActivityStore::ActivityStore(
	IndexType     iSize
):
iHead(0),
tActivites(NULL),
iNumActivites(0L),
iSize(iSize),
iNextSequence(0){
	IA20_TRACER;

	tActivites = new Activity[iSize];

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "New Activity Store, sz: "<<(int)iSize);
}
/*************************************************************************/
ActivityStore::~ActivityStore() throw(){
	IA20_TRACER;

	if(tActivites)
		delete[] tActivites;
}
/*************************************************************************/
Activity* ActivityStore::createActivity( Subscription::HandleType   mSubscriptionHandle,
										 Topic::FullTopicNameHandle aTopicNameHandle,
                              		     Message::HandleType        mMessageHandle,
										 Listener::Task::Command    iCommand,
									     uint8_t                    iQoS){
      
	if(iNumActivites == iSize)
		IA20_THROW(ItemNotFoundException("iNumActivities == iSize")<<(int)iNumActivites<<" "<<(int)iSize);

	Activity* pActivity = nullptr;

	do{
	 	pActivity = tActivites + iHead++;

		if(iHead == iSize){
			iHead = 0;
		}
	
	}while(pActivity->isEmpty());

	new (pActivity) Activity(iNextSequence++,
							 Activity::ST_SendPending, 							 
							 mSubscriptionHandle, 
							 aTopicNameHandle,
							 mMessageHandle,
							 iCommand,
							 iQoS);

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "New Activity ["<<iHead<<"], Sub: "
		<<(void*)(long)mSubscriptionHandle<<":"
		<<(void*)(long)mMessageHandle);

	lstPendingActivities.push_front(pActivity);

	iNumActivites++;

	return pActivity;
}
/*************************************************************************/
}
}
