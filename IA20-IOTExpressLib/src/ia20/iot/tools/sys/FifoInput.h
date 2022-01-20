/*
 * File: FifoInput.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_FifoInput_H_
#define _IA20_IOT_Tools_SYS_FifoInput_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>
#include "Fifo.h"

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {



/*************************************************************************/
/** The FifoInput class.
 *
 */
template<class T>
class FifoInput : 
	public Fifo<T>, 	
	public IA20::URing::IO::ReadHandler {

public:


	FifoInput(size_t iBufferSize, URing::RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
	Fifo<T>::Fifo(iBufferSize),
	ReadHandler(pRingHandler, pFileHandle),
	pCursor(Fifo<T>::tabBuffer),
	bInProgress(false),
	pIOReadHead(Fifo<T>::tabBuffer){};


	bool dequeue(T& v){

		if(pCursor == pIOReadHead){
			prepareRead(); //TODO read ahead
			return false;
		}

		v = *pCursor++;

		if(pCursor >= Fifo<T>::pGuard){
			pCursor = Fifo<T>::tabBuffer;
		}

		// IA20_LOG(true, "dequeue: "<<(void*)v);

		return true;
	}

	void schedule(){
		prepareRead();
	}

protected:

	T* pCursor;
	T* pIOReadHead;

	void prepareRead(){

		if(bInProgress)
			return;

		size_t iRequestSize = Fifo<T>::CChunkSize;
		
		if(pIOReadHead + iRequestSize >= Fifo<T>::pGuard){
			iRequestSize = Fifo<T>::pGuard - pIOReadHead;
		}

		if( pIOReadHead < pCursor && 
			pCursor < pIOReadHead + iRequestSize){
			iRequestSize = pCursor - pIOReadHead;
		}

		if(iRequestSize == 0)
			return;

		URing::IO::ReadHandler::iovec.iov_base = pIOReadHead;
		URing::IO::ReadHandler::iovec.iov_len  = iRequestSize * sizeof(T);
		URing::IO::ReadHandler::prepare();
		bInProgress = true;
	}

	virtual void handleRead(off_t iDataLen){
		bInProgress = false;
		pIOReadHead += iDataLen / sizeof(T);
		
		if(pIOReadHead >= Fifo<T>::pGuard){
			pIOReadHead = Fifo<T>::tabBuffer;
		}

		IA20_LOG(true, "Read: "<<iDataLen<<", IO offset: "
			<<(pIOReadHead-Fifo<T>::tabBuffer)<<" Read offset: "
			<<(pCursor-Fifo<T>::tabBuffer));

	}

	bool bInProgress;
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_FifoInput_H_ */
