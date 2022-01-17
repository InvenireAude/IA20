/*
 * File: TopicsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "TopicsStore.h"
#include "Subscription.h"

#include <ia20/iot/tools/WordsMap.h>
#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
TopicsStore::TopicsStore():
	hmChild(1000),
	ptrWordsMap(new Tools::WordsMap()){
	IA20_TRACER;
	pRootTopic = TopicPool::New(Tools::WordsMap::CRoot, nullptr);
}
/*************************************************************************/
TopicsStore::~TopicsStore() throw(){
	IA20_TRACER;

	for(auto it : hmChild){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"TopicStore dump:["<<
			it.first.first<<","<<it.first.second<<"]="<<(void*)it.second);
	}
}
/*************************************************************************/
Topic *TopicsStore::getOrCreateTopic(const Tools::StringRef& strTopic){
	IA20_TRACER;

	Tools::WordTokens tokens;
	tokens.read(strTopic, ptrWordsMap.get());

//	Tools::WordsMap::WordIdType iParentToken = pRootTopic->getToken();

	Topic *pCursor = pRootTopic;

	for(Tools::WordTokens::const_iterator it = tokens.begin();
			it != tokens.end(); ++it){

		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Get topic - token: "<<(int)*it);

		KeyType key(pCursor, *it);

		ChildMap::iterator itNext = hmChild.find(key);

		if(itNext == hmChild.end()){
			pCursor = TopicPool::New(*it, pCursor);

			if(*it != Tools::WordsMap::CHash && *it != Tools::WordsMap::CPlus)
				hmChildren[key.first].push_back(pCursor);
			
			hmChild[key] = pCursor;
		}else{
			pCursor = itNext->second;
		}
	}

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Get Topic "<<strTopic<<" = "<<(void*)pCursor);

	return pCursor;
}
/*************************************************************************/
void TopicsStore::iterate(const Tools::StringRef& strTopic, 
						  Topic::Callback*        pCallback){

	  Tools::WordTokens tokens;
	  tokens.read(strTopic, ptrWordsMap.get());

	  std::list<Topic*> lstCursors;
	  lstCursors.push_back(pRootTopic);

	  //TODO this std::list is overkill, temporary hacks
	  // note, we prepend pointers in the same list during the loop
      // consider array, traverse backward from n -> 0
	  // appends will be read on the next iteration
	  // delete by null or swap with the n-th - better.

	  for(Tools::WordTokens::const_iterator it = tokens.begin(); 
        	!lstCursors.empty() && it != tokens.end(); ++it){


		for(std::list<Topic*>::iterator itCursor = lstCursors.begin();
			itCursor != lstCursors.end();){

			KeyType keyHash(*itCursor,  Tools::WordsMap::CHash);

			ChildMap::const_iterator itNextHash = hmChild.find(keyHash);
			if(itNextHash != hmChild.end()){
				itNextHash->second->iterate(pCallback);
			}

			KeyType keyPlus(*itCursor,  Tools::WordsMap::CPlus);

			ChildMap::const_iterator itNextPlus = hmChild.find(keyPlus);
			if(itNextPlus != hmChild.end()){
				lstCursors.push_front(itNextPlus->second);
			}

			KeyType key(*itCursor, *it);
			ChildMap::const_iterator itNext = hmChild.find(key);

			if(itNext == hmChild.end()){
		  		itCursor = lstCursors.erase(itCursor);
			}else{
			  	*itCursor = itNext->second;
				itCursor++;
			}
		}

	  }

	for(std::list<Topic*>::iterator itCursor = lstCursors.begin();
			itCursor != lstCursors.end(); itCursor++){
		(*itCursor)->iterate(pCallback);
	}
}
/*************************************************************************/
}
}
