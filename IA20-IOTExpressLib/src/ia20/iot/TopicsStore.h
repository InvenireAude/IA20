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

	void iterateRetained(const Tools::StringRef& strTopic, 
				 		 Topic::RetainCallback* pCallback);

	
	inline Tools::StringRef getFullTopicName(Topic::FullTopicNameHandle iNameHandle)const{
		FullTopicNamesRevMap::const_iterator it = hmFullTopicNamesRev.find(iNameHandle);
		
		if(it == hmFullTopicNamesRev.end()){
			IA20_THROW(ItemNotFoundException("FullTopicNameHandle: ")<<(long)iNameHandle);
		}

		return it->second;
	};

	inline Topic::FullTopicNameHandle getTopicNameHandle(const Tools::StringRef& strTopic)const{
		FullTopicNamesMap::const_iterator it = hmFullTopicNames.find(String(strTopic));
		
		if(it == hmFullTopicNames.end()){
			IA20_THROW(ItemNotFoundException("FullTopicName: ")<<strTopic);
		}

		return it->second;
	};

	inline Topic::FullTopicNameHandle addFullTopicName(const Tools::StringRef& strTopic){

		Topic::FullTopicNameHandle iHandle = hmFullTopicNames.size() + 1;

		hmFullTopicNames[strTopic] = iHandle;
		hmFullTopicNamesRev[iHandle] = strTopic;

		return iHandle;
	};

protected:

	typedef Memory::FixedObjectsPool<Topic, 10000> TopicPool;
	typedef std::pair<Topic*, Tools::WordsMap::WordIdType> KeyType;

	struct Hash{
		size_t operator()(KeyType key)const{
			return (uint64_t)key.first + key.second;
		}
	};

	typedef std::unordered_map<KeyType, Topic*, Hash> ChildMap;
	typedef std::unordered_map<Topic*,  std::list<Topic*> > ChildrenMap;

	ChildMap    hmChild;
	ChildrenMap hmChildren;

	Topic* pRootTopic;

	std::unique_ptr<Tools::WordsMap> ptrWordsMap;
	
	//TODO temporary

	typedef std::unordered_map<String, Topic::FullTopicNameHandle, _Hash<String>> FullTopicNamesMap;
	typedef std::unordered_map<Topic::FullTopicNameHandle, String> FullTopicNamesRevMap;

	FullTopicNamesMap hmFullTopicNames;
	FullTopicNamesRevMap hmFullTopicNamesRev;

	class RetainedWorker {
		
		public:

		inline RetainedWorker(const Tools::StringRef& strTopic, 
		                      TopicsStore*  pTopicsStore,
							  Topic::RetainCallback*   pCallback):
			pCallback(pCallback),
			pTopicsStore(pTopicsStore){
				tokens.read(strTopic, pTopicsStore->getWordsMap());
		}

		void iterateAll(Topic* pCursor);

		void iterateRetained(Tools::WordTokens::const_iterator it,
							 Topic* pCursor);

		void iterateRetained();

		protected:

		Tools::WordTokens        tokens;
		TopicsStore*             pTopicsStore;
		Topic::RetainCallback*   pCallback;
	};
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_TopicsStore_H_ */
