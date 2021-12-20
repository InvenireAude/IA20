/*
 * File: TasksRing.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_TasksRing_H_
#define _IA20_IOT_Tools_SYS_TasksRing_H_

#include <ia20/commonlib/commonlib.h>
#include "../TasksRing.h"


#include <vector>

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
/** The TasksRing class.
 *
 */
template<class D>
class TasksRing : public IA20::IOT::Tools::TasksRing<D> {
public:

	virtual ~TasksRing() throw(){
  };

/*************************************************************************/
TasksRing(unsigned int iSize):
  vValues(iSize),
  iLength(0),
  iHead(0){
	IA20_TRACER;
}
/*************************************************************************/
virtual void enque(D* pValue){
	IA20_TRACER;

  Mutex::Locker locker(mutex);

  while(vValues.size() == iLength)
    mutex.wait(cndWriter, 1000);

  vValues[ (iHead + iLength++) % vValues.size() ] = pValue;
  cndReader.broadcast();
  IA20_LOG(false, "Enque: "<<(void*)this<<"\t"<<iHead<<", "<<iLength);
}
/*************************************************************************/
virtual D *deque(){
	IA20_TRACER;

  IA20_LOG(false, "Deque starts: "<<(void*)this);

  Mutex::Locker locker(mutex);

  while(iLength == 0)
    mutex.wait(cndReader, 1000);

  IA20_LOG(false, "Deque: "<<(void*)this<<"\t"<<iHead<<", "<<iLength);

  D *pValue = vValues[ iHead % vValues.size() ];

  if(iHead < vValues.size() - 1){
    iHead++;
  }else{
    iHead = 0;
  }
  iLength--;
  cndWriter.broadcast();

  return pValue;
}

/*************************************************************************/
  static std::unique_ptr< typename Tools::TasksRing<D>::Interface> CreateInterface(unsigned int iSize){

     std::unique_ptr<typename Tools::TasksRing<D> > ptr1(new TasksRing(iSize));
     std::unique_ptr<typename Tools::TasksRing<D> > ptr2(new TasksRing(iSize));

    return std::unique_ptr< typename Tools::TasksRing<D>::Interface>(
        new typename Tools::TasksRing<D>::Interface(std::move(ptr1),std::move(ptr2)));

  };

/*************************************************************************/
protected:

  Mutex mutex;

  Condition cndWriter;
  Condition cndReader;

  typedef std::vector<D*> ValuesVector;
  ValuesVector vValues;

  unsigned int iHead;
  unsigned int iLength;


};
/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_TasksRing_H_ */

