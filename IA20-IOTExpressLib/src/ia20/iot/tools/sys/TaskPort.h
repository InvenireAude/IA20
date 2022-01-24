/*
 * File: TaskPort.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_TaskPort_H_
#define _IA20_IOT_Tools_SYS_TaskPort_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>

#include "../TaskPort.h"

#include "FifoInput.h"
#include "FifoOutput.h"

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
/** The TaskPort class.
 *
 */
extern Net::Conn::TCP::Peer __peer;

template<class T>
class TaskPort :  Tools::TaskPort<T>{
public:

	virtual ~TaskPort() throw(){};

	TaskPort(size_t iSize, URing::RingHandler *pRingHandler, int fdIn, int fdOut):
	    pRingHandler(pRingHandler),
		ptrFileHandleIn(new  Net::Conn::TCP::FileHandle(fdIn, __peer)),
		ptrFileHandleOut(new Net::Conn::TCP::FileHandle(fdOut, __peer)){

		ptrInput.reset( new FifoInput<T>(iSize, pRingHandler,  ptrFileHandleIn.get()));
		ptrOutput.reset( new FifoOutput<T>(iSize, pRingHandler, ptrFileHandleOut.get()));
	}

 	virtual bool dequeue(T& v){
		return ptrInput->dequeue(v);
	 }
  	
	virtual bool enqueue(T v){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsDetailedInfo, "enqueue: "<<(void*)this);
		return ptrOutput->enqueue(v);
	}

	virtual void flush(){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsDetailedInfo, "flush: "<<(void*)this);
		ptrOutput->flush();
	}

	virtual void schedule(){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsDetailedInfo, "schedule: "<<(void*)this);
		ptrInput->schedule();
	}
/*************************************************************************/
  static std::unique_ptr< typename Tools::TaskPort<T> > 
  	Create(size_t iSize, URing::RingHandler *pRingHandler, int fdIn, int fdOut){

     return std::unique_ptr< typename Tools::TaskPort<T> >(
		 new TaskPort(iSize, pRingHandler, fdIn, fdOut));

  };

/*************************************************************************/

	inline URing::RingHandler *getRingHandler()const{
		return pRingHandler;
	}

protected:

	std::unique_ptr<Net::Conn::TCP::FileHandle>  ptrFileHandleIn;
    std::unique_ptr<Net::Conn::TCP::FileHandle>  ptrFileHandleOut;

	std::unique_ptr< FifoInput<T> > ptrInput;
	std::unique_ptr< FifoOutput<T> > ptrOutput;

	URing::RingHandler *pRingHandler;
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_TaskPort_H_ */
