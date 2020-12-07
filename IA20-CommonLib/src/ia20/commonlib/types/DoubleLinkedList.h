/*
 * File: IA20-CommonLib/src/commonlib/types/DoubleLinkedList.h
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
#ifndef _IA20_Types_DoubleLinkedList_H_
#define _IA20_Types_DoubleLinkedList_H_

#include <ia20/commonlib/types.h>

namespace IA20 {

/*************************************************************************/
/** The DoubleLinkedList class.
 *
 */
template<class C>
class DoubleLinkedList {

public:

  DoubleLinkedList(C* pValue):
    pPrev(this),
    pNext(this),
    pValue(pValue){}

 ~DoubleLinkedList(){
    if(pNext != this)
      this->pNext->pPrev = this->pPrev;

    if(pPrev != this)
      this->pPrev->pNext = this->pNext;
 }

  void append(C *pNext){
    pNext->pNext = this->pNext;
    this->pNext->pPrev = pNext;
    this->pNext = pNext;
    pNext->pPrev = this;
  }

  void prepend(C *pPrev){
    pPrev->pPrev = this->pPrev;
    this->pPrev->pNext = pPrev;
    this->pPrev = pPrev;
    pPrev->pNext = this;
  }

  void detach(){

    if(pNext != this)
      this->pNext->pPrev = this->pPrev;

    if(pPrev != this)
      this->pPrev->pNext = this->pNext;

    this->pPrev = this->pNext = this;
  }

inline DoubleLinkedList<C> *getNext() { return pNext;};
inline DoubleLinkedList<C> *getPrev() {return pPrev;};
inline C *getValue() {return pValue;};

inline const DoubleLinkedList<C> *getNext()const { return pNext;};
inline const DoubleLinkedList<C> *getPrev()const {return pPrev;};
inline const C *getValue()const {return pValue;};



operator C&(){ return *pValue;}
operator const C&()const { return *pValue;}

class iterator {
   public:
    iterator(const iterator& other):
      pCurrent(other.pCurrent){}

    bool operator==(const iterator& other){
      return pCurrent == other.pCurrent;
    }

    bool operator!=(const iterator& other){
      return pCurrent != other.pCurrent;
    }

    iterator& operator=(const iterator& other){
      pCurrent = other.pCurrent;
      return *this;
    };

  iterator& operator++(){

    pCurrent = pCurrent->pNext;

    if(!pCurrent->pValue)
      pCurrent = NULL;

    return *this;
  }

  C* operator*() const{
      IA20_CHECK_IF_NULL(pCurrent);
      return pCurrent->pValue;
    }

  protected:

   iterator(DoubleLinkedList<C> *pCurrent):
    pCurrent(pCurrent){}

    DoubleLinkedList<C> *pCurrent;

    friend class  DoubleLinkedList<C>;
  };

  iterator begin(){
    return iterator(this->pValue ? this : NULL);
  }

  iterator end(){
    return iterator(NULL);
  }


 protected:

  iterator begin(DoubleLinkedList<C> *pCurrent){
    if(!pCurrent->pValue)
      pCurrent = NULL;
    return iterator(pCurrent);
  }

 private:

  DoubleLinkedList<C> *pPrev;
  DoubleLinkedList<C> *pNext;
  C                   *pValue;
};

/*************************************************************************/

template<class C>
class DoubleLinkedListOwner : public DoubleLinkedList<C> {

  public:
    DoubleLinkedListOwner():DoubleLinkedList<C>(NULL){};

    typedef typename IA20::DoubleLinkedList<C>::iterator iterator;

    iterator begin(){
      return IA20::DoubleLinkedList<C>::begin(DoubleLinkedList<C>::getNext());
    };

    iterator remove(iterator& it){
      if(it == DoubleLinkedList<C>::end())
        return it;

      DoubleLinkedList<C> *pNext = (*it)->getNext();

      (*it)->detach();

      return IA20::DoubleLinkedList<C>::begin(pNext);
    }

 };

}

#endif /* _IA20_Types_DoubleLinkedList_H_ */
