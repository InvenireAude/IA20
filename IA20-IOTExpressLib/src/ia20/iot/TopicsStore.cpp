/*
 * File: TopicsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "TopicsStore.h"
#include <ia20/iot/tools/WordsMap.h>

#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
TopicsStore::TopicsStore():
	hmChildren(1000),
	ptrWordsMap(new Tools::WordsMap()){
	IA20_TRACER;
	pRootTopic = TopicPool::New(Tools::WordsMap::CRoot, nullptr);
}
/*************************************************************************/
TopicsStore::~TopicsStore() throw(){
	IA20_TRACER;

	for(auto it : hmChildren){
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

		ChildMap::iterator itNext = hmChildren.find(key);

		if(itNext == hmChildren.end()){
			pCursor = TopicPool::New(*it, pCursor);
			hmChildren[key] = pCursor;
		}else{
			pCursor = itNext->second;
		}
	}

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Get Topic "<<strTopic<<" = "<<(void*)pCursor);

	return pCursor;
}
/*************************************************************************/
bool TopicsStore::lookup(const Tools::StringRef& strTopic, Topic* &refPtrResult)const{

	  Tools::WordTokens tokens;
	  tokens.read(strTopic, ptrWordsMap.get());

	  Topic* pCursor = pRootTopic;

	  for(Tools::WordTokens::const_iterator it = tokens.begin(); 
        	pCursor && it != tokens.end(); ++it){

		KeyType key(pCursor, *it);

		ChildMap::const_iterator itNext = hmChildren.find(key);

		if(itNext == hmChildren.end()){
		  pCursor = nullptr;
		}else{
		  pCursor = itNext->second;
		}

	  }

	if(pCursor){
		refPtrResult = pCursor;
		return true;
	}

	return false;
}

/*************************************************************************/
}
}
