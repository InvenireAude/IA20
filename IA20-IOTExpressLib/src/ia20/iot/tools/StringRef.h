/*
 * File: StringRef.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_StringRef_H_
#define _IA20_IOT_Tools_StringRef_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The StringRef class.
 *
 */
class StringRef {
public:


	class const_iterator{
		public:

			const_iterator(const uint8_t* pCursor):pCursor(pCursor){};

			inline bool operator==(const const_iterator& o)const{
				return o.pCursor == pCursor;
			}

			inline bool operator!=(const const_iterator& o)const{
				return o.pCursor != pCursor;
			}

			inline const_iterator operator=(const const_iterator& o){
				pCursor = o.pCursor;
				return *this;
			}

			const uint8_t operator*()const{
				return *pCursor;
			}

			const_iterator& operator++(){
				pCursor++;
				return *this;
			}

		protected:
			const uint8_t* pCursor;

		friend class StringRef;
	};

	StringRef(const uint8_t* pData, int iLength):pData(pData),iLength(iLength){}
	StringRef(const_iterator itStart, const_iterator itEnd):
			pData(itStart.pCursor),
			iLength(itEnd.pCursor - itStart.pCursor){}

	const_iterator begin()const{
		return const_iterator(pData);
	}

	const_iterator end()const{
		return pData + iLength;
	}

	operator String()const{
		return String((const char*)pData, iLength);
	}

  friend std::ostream& operator<<(std::ostream& os, const StringRef& s){
    os<<s.operator IA20::String();
    return os;
  }

protected:
	const uint8_t* pData;
 	int iLength;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_StringRef_H_ */
