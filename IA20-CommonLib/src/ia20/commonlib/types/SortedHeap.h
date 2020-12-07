/*
 * File: IA20-CommonLib/src/commonlib/types/SortedHeap.h
 *
 * Copyright (C) 2020, Albert Krzymowski
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
#ifndef _IA20_Types_SortedHeap_H_
#define _IA20_Types_SortedHeap_H_

#include <ia20/commonlib/types.h>
#include <ia20/commonlib/memory/memory.h>
#include <ia20/commonlib/exception/BadUsageException.h>

namespace IA20 {

/*************************************************************************/
/** The DoubleLinkedList class.
 *
 */
template<class C, class CO = std::less<C>, class TA = IA20_DFT_FACTORY<C>  >
class SortedHeap {

public:

  SortedHeap(C *pStart, size_t iSize):
    tabValues(pStart),
    iSize(iSize),
    iNextFree(0){}

  SortedHeap(size_t iSize):
    iSize(iSize),
    iNextFree(0){
      TA a;
      tabValues = a.allocate(iSize);
    }

  const C& getMin(){
      if(iNextFree == 0)
        IA20_THROW(BadUsageException("SortedHeap: is empty."));
      return tabValues[0];
  }

  void insert(const C& aValue){

    if(iNextFree == iSize)
      IA20_THROW(BadUsageException("SortedHeap: out of range."));

    size_t iCurrent = iNextFree;
    tabValues[iNextFree++] = aValue;

    while(iCurrent > 0){

      C& c = tabValues[iCurrent];
      size_t iParent = (iCurrent - 1) >> 1;
      C& p = tabValues[iParent];

      if( co(c, p) ){
        C t(c);
        c = p;
        p = t;
      }else{
        return;
      }
      iCurrent = iParent;
    };

  };

  void popMin(){

    if(iNextFree == 0)
      IA20_THROW(BadUsageException("SortedHeap: is empty."));

    tabValues[0] = tabValues[--iNextFree];

    size_t iCurrent = 0;
    size_t iChild   = 1;

    while(iChild < iNextFree){

      size_t iSwapMe = iCurrent;

      if(co(tabValues[iChild], tabValues[iCurrent])){
        iSwapMe = iChild;
      }
      if(++iChild < iNextFree && co(tabValues[iChild], tabValues[iSwapMe])){
        iSwapMe = iChild;
      }

      if(iSwapMe == iCurrent){
        return;
      }

      C tmp = tabValues[iCurrent];
      tabValues[iCurrent] = tabValues[iSwapMe];
      tabValues[iSwapMe] = tmp;

      iCurrent = iSwapMe;
      iChild = (iCurrent << 1) + 1;

    };

  };


  void clear(){
    iNextFree = 0;
  }

  size_t size(){
    return iNextFree;
  }

 protected:
  typedef C ItemType;
  C *tabValues;
  size_t iSize;
  CO co;
  size_t iNextFree;
};

/*************************************************************************/

}

#endif /* _IA20_Types_SortedHeap_H_ */
