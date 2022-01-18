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

#include <stack>

namespace IA20 {
namespace IOT {

/*************************************************************************/
TopicsStore::TopicsStore():
	hmChild(1000),
	ptrWordsMap(new Tools::WordsMap()){
	IA20_TRACER;
	pRootTopic = TopicPool::New(Tools::WordsMap::CRoot, nullptr , "");
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
			pCursor = TopicPool::New(*it, pCursor, ptrWordsMap->getName(*it));

			if(*it != Tools::WordsMap::CHash && *it != Tools::WordsMap::CPlus){
				hmChildren[key.first].push_back(pCursor);
			}
			
			hmChild[key] = pCursor;
		}else{
			pCursor = itNext->second;
		}
	}

  	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Get Topic: "<<strTopic<<" = "<<(void*)pCursor);
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
void TopicsStore::RetainedWorker::iterateAll(Topic* pCursor){

 	for(const auto& c: pTopicsStore->hmChildren[pCursor]){
		iterateAll(c);
	}

	if(pCursor && pCursor->hasRetained()){
		pCallback->onTopic(pCursor);
	}
}
/*************************************************************************/													
void TopicsStore::RetainedWorker::iterateRetained(Tools::WordTokens::const_iterator it,
								  				  Topic* pCursor){
						
	  while(pCursor && it != tokens.end()){

		  	if(*it == Tools::WordsMap::CPlus){
				  ++it;
				  for(const auto& c: pTopicsStore->hmChildren[pCursor]){
					  iterateRetained(it, c);
				  }
				return;

			}else if(*it == Tools::WordsMap::CHash){				  
				  for(const auto& c: pTopicsStore->hmChildren[pCursor]){
					  iterateAll(c);
				  }
				return;
			}else{

				KeyType key(pCursor, *it);
				ChildMap::const_iterator itNext = pTopicsStore->hmChild.find(key);

				if(itNext == pTopicsStore->hmChild.end()){
					pCursor = nullptr;
				}else{
					pCursor = itNext->second;						
				}
			}
		++it;
	  }

	if(pCursor && pCursor->hasRetained()){
		pCallback->onTopic(pCursor);
	}

}
/*************************************************************************/
void TopicsStore::RetainedWorker::iterateRetained(){
	iterateRetained(tokens.begin(), pTopicsStore->pRootTopic);
}
/*************************************************************************/
void TopicsStore::iterateRetained(const Tools::StringRef& strTopic, 
						  		 Topic::RetainCallback*   pCallback){

	

	RetainedWorker worker(strTopic, this, pCallback);
	worker.iterateRetained();
}
/*************************************************************************/
}
}
/*
*/



/*
void TopicsStore::iterateRetained(const Tools::StringRef& strTopic, 
						  		 Topic::RetainCallback*   pCallback){

 	  IA20_LOG(true, "What ?: \t"<<strTopic);

	  Tools::WordTokens tokens;
	  tokens.read(strTopic, ptrWordsMap.get());

 	  std::list<Topic*> lstCursors;
	  lstCursors.push_back(pRootTopic);

	 for(Tools::WordTokens::const_iterator it = tokens.begin(); 
        	!lstCursors.empty() && it != tokens.end(); ++it){
			
		IA20_LOG(true, "====" );

		for(std::list<Topic*>::iterator itCursor = lstCursors.begin();
			itCursor != lstCursors.end();){

		 	IA20_LOG(true, "i: "<<(int)(*it)<<" "<<(*itCursor)->getName());

		  	if(*it == Tools::WordsMap::CPlus){
				  for(const auto& c: hmChildren[*itCursor]){
					  lstCursors.push_back(c);
				  }
			}else{

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
	  }

	for(std::list<Topic*>::iterator itCursor = lstCursors.begin();
			itCursor != lstCursors.end(); itCursor++)
		if(*itCursor && (*itCursor)->hasRetained()){
			pCallback->onTopic(*itCursor);
		}
		

}*/
