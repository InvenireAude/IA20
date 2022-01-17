/*
 * File: TopicsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_TopicsStore_H_
#define _IA20_IOT_TopicsStore_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/FixedObjectsPool.h>
#include <ia20/iot/tools/WordsMap.h>
#include <ia20/iot/tools/WordTokens.h>

#include "Topic.h"

#include <unordered_map>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The TopicsStore class.
 *
 */
class TopicsStore {
public:

	virtual ~TopicsStore() throw();
	TopicsStore();

	Topic *getOrCreateTopic(const Tools::StringRef& strTopic);
	
	Topic *getRoot()const{
		return pRootTopic;
	}

	bool lookup(const Tools::StringRef& strTopic, Topic* &refPtrResult)const;

	Tools::WordsMap* getWordsMap()const{
		return ptrWordsMap.get();
	}

	void iterate(const Tools::StringRef& strTopic, 
				 Topic::Callback* pCallback);
	
protected:

	typedef Memory::FixedObjectsPool<Topic, 10000> TopicPool;
	typedef std::pair<Topic*, Tools::WordsMap::WordIdType> KeyType;

	struct Hash{
		size_t operator()(KeyType key)const{
			return (uint64_t)key.first + key.second;
		}
	};

	typedef std::unordered_map<KeyType, Topic*, Hash> ChildMap;

	ChildMap hmChildren;

	Topic* pRootTopic;

	std::unique_ptr<Tools::WordsMap> ptrWordsMap;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_TopicsStore_H_ */
