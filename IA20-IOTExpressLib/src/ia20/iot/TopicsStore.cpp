/*
 * File: TopicsStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "TopicsStore.h"
#include <ia20/iot/tools/WordsMap.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
TopicsStore::TopicsStore():
	hmChildren(1000),
	ptrWordsMap(new Tools::WordsMap()){
	IA20_TRACER;
	pRootTopic = TopicPool::New(Tools::WordsMap::CRoot);
}
/*************************************************************************/
TopicsStore::~TopicsStore() throw(){
	IA20_TRACER;

	for(auto it : hmChildren){
		IA20_LOG(true,"TopicStore dump:["<<
			it.first.first<<","<<it.first.second<<"]="<<(void*)it.second);
	}
}
/*************************************************************************/
Topic *TopicsStore::getTopic(const Tools::StringRef& strTopic){
	IA20_TRACER;

	Tools::WordTokens tokens;
	tokens.read(strTopic, ptrWordsMap.get());

//	Tools::WordsMap::WordIdType iParentToken = pRootTopic->getToken();
	
	Topic *pCursor = pRootTopic;

	for(Tools::WordTokens::const_iterator it = tokens.begin(); 
			it != tokens.end(); ++it){

		IA20_LOG(true,"Subscription token: "<<(int)*it);

		KeyType key(pCursor, *it);
		
		ChildMap::iterator itNext = hmChildren.find(key);
		
		if(itNext == hmChildren.end()){
			pCursor = TopicPool::New(*it);
			hmChildren[key] = pCursor;
		}else{
			pCursor = itNext->second;
		}
	}

	return pCursor;
}
/*************************************************************************/
Topic* TopicsStore::lookup(Topic* pParent, Tools::WordsMap::WordIdType iWordId)const{
	IA20_TRACER;

	KeyType key(pParent, iWordId);

	ChildMap::const_iterator it = hmChildren.find(key);

	if(it == hmChildren.end())
		IA20_THROW(ItemNotFoundException("Topic: [")<<(int)pParent->getToken()<<","<<(int)iWordId<<"]");

	return it->second;
}
/*************************************************************************/
}
}
