/*
 * File: IA20-CommonLib/src/commonlib/types/BitMap.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _IA20_Types_BitMap_H_
#define _IA20_Types_BitMap_H_

#include <list>


namespace IA20 {

/*************************************************************************/
/** The BitMap class.
 *
 */
class BitMap {
public:

	virtual ~BitMap() throw();

	bool getBit(unsigned int iIdx);
	void setBit(unsigned int iIdx, bool bValue=true);

	void makeOr(const BitMap* pOther);
	void makeAnd(const BitMap* pOther);
	void clear();

	int getSize() const;

	typedef std::list<unsigned int> BitsList;

	void getBits(BitsList& refOutputList);

	BitMap(unsigned int iSize);
	BitMap(const BitMap* pSource);

protected:

	typedef unsigned long Word;

	Word *tValues;

	unsigned int iSize;

};

/*************************************************************************/

}

#endif /* _IA20_Types_BitMap_H_ */
