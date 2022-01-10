/*
 * File: Topic.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Topic_H_
#define _IA20_IOT_Topic_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/WordsMap.h>

namespace IA20 {
namespace IOT {
class Subscription;

/*************************************************************************/
/** The Topic class.
 *
 */
class Topic {
public:

	//~Topic() throw();

	inline Topic(Tools::WordsMap::WordIdType iToken, Topic* pParent):
	iToken(iToken),
  pParent(pParent),
	pFirst(NULL){
		IA20_LOG(true,"New token: "<<(int)iToken);
	};

	inline Tools::WordsMap::WordIdType getToken()const{
		return iToken;
	}

	void addSubscription(Subscription* pSubscription);

	Subscription* getFirstSubscription()const{
		return pFirst;
	}

	bool hasFirstSubscription()const{
		return pFirst != NULL;
	}

protected:
	Tools::WordsMap::WordIdType iToken;
  Topic* pParent;
	Subscription* pFirst;
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Topic_H_ */
