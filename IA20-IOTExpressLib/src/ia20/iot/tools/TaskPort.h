/*
 * File: TaskPort.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_TaskPort_H_
#define _IA20_IOT_Tools_TaskPort_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The TaskPort class.
 *
 */
template<class T>
class TaskPort {
public:

	// class Interface{
    
	// public:
	
  //     typedef std::unique_ptr<TaskPort> PtrType;

  //     Interface(PtrType&& ptrClientPort, PtrType&& ptrServerPort):
  //       ptrClientPort(std::move(ptrClientPort)),
  //       ptrServerPort(std::move(ptrServerPort)){
  //       }

  //     inline TaskPort* getClientPort(){
  //       return ptrClientPort.get();
  //     }

  //     inline TaskPort* getServerPort(){
  //       return ptrServerPort.get();
  //     }

  //   protected:
  //     PtrType ptrClientPort;
  //     PtrType ptrServerPort;
  // };

  virtual ~TaskPort() throw(){};
  TaskPort(){};

  virtual bool dequeue(T& ptr)=0;
  virtual bool enqueue(T v)=0;
  virtual void flush()=0;
  virtual void schedule()=0;

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_TaskPort_H_ */
