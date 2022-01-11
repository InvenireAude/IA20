/*
 * File: TasksRing.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_TasksRing_H_
#define _IA20_IOT_Tools_TasksRing_H_

#include <ia20/commonlib/commonlib.h>

#include <memory>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The TasksRing class.
 *
 */

template<class D>
class TasksRing {
public:

	virtual ~TasksRing() throw(){};
	TasksRing(){};

  virtual void  enque(D*) = 0;
  
  virtual D* deque() = 0;
  virtual D* dequeNoWait() = 0;

  class Interface{
    public:
      typedef std::unique_ptr< TasksRing > PtrType;

      Interface(PtrType&& ptrRequests, PtrType&& ptrResponses):
        ptrRequests(std::move(ptrRequests)),
        ptrResponses(std::move(ptrResponses)){
        }

      inline TasksRing* getRequests(){
        return ptrRequests.get();
      }

      inline TasksRing* getResponses(){
        return ptrResponses.get();
      }

    protected:
      PtrType ptrRequests;
      PtrType ptrResponses;
  };

};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_TasksRing_H_ */

