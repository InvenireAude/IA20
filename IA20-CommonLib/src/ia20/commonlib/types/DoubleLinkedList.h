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
    pNext->pPrev = this;

    this->pNext->pPrev = pNext;
    this->pNext = pNext;
  }

  void prepend(C *pPrev){

    pPrev->pPrev = this->pPrev;
    pPrev->pNext = this;

    this->pPrev->pNext = pPrev;
    this->pPrev = pPrev;

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

    template<class IT>
    class iterator_base {

      public:
        iterator_base(const iterator_base& other):
          pCurrent(other.pCurrent){}

        bool operator==(const iterator_base& other){
          return pCurrent == other.pCurrent;
        }

        bool operator!=(const iterator_base& other){
          return pCurrent != other.pCurrent;
        }

        iterator_base& operator=(const iterator_base& other){
          pCurrent = other.pCurrent;
          return *this;
        };

      iterator_base& operator++(){

        pCurrent = pCurrent->pNext;

        if(!pCurrent->pValue)
          pCurrent = NULL;

        return *this;
      }

      IT operator*() const{
          IA20_CHECK_IF_NULL(pCurrent);
          return pCurrent->pValue;
        }

      iterator_base(DoubleLinkedList<C> *pCurrent):
          pCurrent(pCurrent){}

      protected:

        DoubleLinkedList<C> *pCurrent;

        friend class  DoubleLinkedList<C>;
      };

      typedef iterator_base<C*> iterator;
      typedef iterator_base<const C*> const_iterator;

      iterator begin(){
        return iterator(this->getNext());
      }

      iterator end(){
        return iterator(this);
      }

      const_iterator begin()const{
        return const_iterator(this->getPrev());
      }

      const_iterator end()const{
        return const_iterator(this);
      }


 protected:

    iterator begin(DoubleLinkedList<C> *pCurrent){
      if(!pCurrent->pValue)
        pCurrent = NULL;
      return iterator(pCurrent);
    }

    const_iterator begin(DoubleLinkedList<C> *pCurrent)const{
      if(!pCurrent->pValue)
        pCurrent = NULL;
      return iterator(pCurrent);
    }

    typename DoubleLinkedList<C>::iterator remove(typename DoubleLinkedList<C>::iterator& it){
      if(it == DoubleLinkedList<C>::end())
        return it;

      DoubleLinkedList<C> *pNext = (*it)->getNext();

      (*it)->detach();

      return IA20::DoubleLinkedList<C>::begin(pNext);
    }

  bool empty() const{
    return DoubleLinkedList<C>::getNext()->getValue() == NULL;
  }

 };

}

#endif /* _IA20_Types_DoubleLinkedList_H_ */
