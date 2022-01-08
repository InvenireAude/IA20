/*
 * File: WordTokens.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "WordTokens.h"


namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
WordTokens::WordTokens():
iSize(0){
	IA20_TRACER;
}
/*************************************************************************/
WordTokens::~WordTokens() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void WordTokens::read(const StringRef& strValue, WordsMap* pWordsMap){
	IA20_TRACER;

	Tools::StringRef::const_iterator itCursor = strValue.begin();
	Tools::StringRef::const_iterator itStart  = itCursor;

	do{

		while(*itStart == '/'){
			++itStart;
			itCursor = itStart;
		}
		
		if(*itCursor){
			++itCursor;
		}
		
		if( (*itCursor == '/' || *itCursor == 0) && itCursor != itStart){
			
			Tools::StringRef strToken(itStart, itCursor);
			
			WordsMap::WordIdType iWord = pWordsMap->lookup(strToken);
			tValues[iSize++] = iWord;
			itStart=itCursor;
		}

	}while(*itCursor != 0);

}
/*************************************************************************/

}
}
}
