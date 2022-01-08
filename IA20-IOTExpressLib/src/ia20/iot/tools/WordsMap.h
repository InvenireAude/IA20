/*
 * File: WordsMap.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_WordsMap_H_
#define _IA20_IOT_Tools_WordsMap_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/StringRef.h>

#include <map>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The WordsMap class.
 *
 */
class WordsMap {
public:

	virtual ~WordsMap() throw();
	WordsMap();


	typedef uint32_t WordIdType;

	WordIdType lookup(const Tools::StringRef& strValue);


	static const WordIdType CRoot = 0;
	static const WordIdType CHash = 1;
	static const WordIdType CPlus = 2;
protected:

 //TODO temporary implementation with STL and String (change to UTFStrReference(s, len) )

	typedef std::map<String, WordIdType> ValuesMap;
	ValuesMap hmValues;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_WordsMap_H_ */
