/*
 * File: WordsMap.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "WordsMap.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
WordsMap::WordsMap(){
	IA20_TRACER;

	hmValues[""] = 0;
	hmValues["#"] = 1;
	hmValues["+"] = 2;

	hmNames[0] = "";
	hmNames[1] = "#";
	hmNames[2] = "+";

}
/*************************************************************************/
WordsMap::~WordsMap() throw(){
	IA20_TRACER;
}
/*************************************************************************/
WordsMap::WordIdType WordsMap::lookup(const Tools::StringRef& strValue){
	IA20_TRACER;

	ValuesMap::const_iterator it = hmValues.find(strValue);
	
	if( it != hmValues.end() ){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Got token: ["<<(String)strValue<<"]="<<it->second);
		return it->second;
	}else{
		WordIdType iWord = hmValues.size();
		hmValues[strValue] = iWord;
		hmNames[iWord]     = String(strValue);
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "New token: ["<<(String)strValue<<"]="<<iWord);
		return iWord;
	}

}
/*************************************************************************/
}
}
}
