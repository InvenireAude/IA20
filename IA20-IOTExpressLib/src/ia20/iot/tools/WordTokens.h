/*
 * File: WordTokens.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_WordTokens_H_
#define _IA20_IOT_Tools_WordTokens_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/StringRef.h>

#include "WordsMap.h"

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The WordTokens class.
 *
 */
class WordTokens {
public:

	~WordTokens() throw();
	WordTokens();

	static const int CMaxSize = 16;

	class const_reverse_iterator {
		public:

		inline bool operator==(const const_reverse_iterator& o)const{
			return o.iIdx == iIdx && &wt == &o.wt;
		}

		inline bool operator!=(const const_reverse_iterator& o)const{
			return !operator==(o);
		}

		inline const_reverse_iterator& operator++(){
			if(iIdx >=0 )
				iIdx--;
			return *this;
		}

		inline WordsMap::WordIdType operator*()const{
			return wt.tValues[iIdx];
		}

		protected:
			inline const_reverse_iterator(const WordTokens& wt, int iIdx):wt(wt),iIdx(iIdx){}
			
			int iIdx;
			const WordTokens& wt;

			friend class WordTokens;
	};

	class const_iterator {
		public:

		inline bool operator==(const const_iterator& o)const{
			return o.iIdx == iIdx && &wt == &o.wt;
		}

		inline bool operator!=(const const_iterator& o)const{
			return !operator==(o);
		}

		inline const_iterator& operator++(){
			if(iIdx < wt.iSize)
				iIdx++;
			return *this;
		}

		inline WordsMap::WordIdType operator*()const{
			return wt.tValues[iIdx];
		}

		protected:
			inline const_iterator(const WordTokens& wt, int iIdx):wt(wt),iIdx(iIdx){}
			
			int iIdx;
			const WordTokens& wt;

			friend class WordTokens;
	};

	inline const_reverse_iterator rbegin()const{
		return const_reverse_iterator(*this, iSize - 1);
	}

	inline const_reverse_iterator rend()const{
		return const_reverse_iterator(*this, -1);
	}

	inline const_iterator begin()const{
		return const_iterator(*this, 0);
	}

	inline const_iterator end()const{
		return const_iterator(*this, iSize);
	}
	
	void read(const StringRef& strValue, WordsMap* pWordsMap);

protected:

	int iSize;
	WordsMap::WordIdType tValues[CMaxSize];
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_WordTokens_H_ */
