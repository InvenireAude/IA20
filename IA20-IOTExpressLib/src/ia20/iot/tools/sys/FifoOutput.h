/*
 * File: FifoOutput.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_FifoOutput_H_
#define _IA20_IOT_Tools_SYS_FifoOutput_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>
#include "Fifo.h"

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
/** The FifoOutput class.
 *
 */
template<class T>
class FifoOutput : public Fifo<T>, 	
	public IA20::URing::IO::WriteHandler {
		
public:

	
	FifoOutput(size_t iBufferSize, URing::RingHandler* pRingHandler, Net::Conn::TCP::FileHandle* pFileHandle):
	Fifo<T>::Fifo(iBufferSize),
	WriteHandler(pRingHandler, pFileHandle),
	iNumPending(0),
	pCursor(Fifo<T>::tabBuffer),
	pIOWriteHead(Fifo<T>::tabBuffer),
	bInProgress(false){};


	bool enqueue(T v){

		IA20_LOG(true, "enqueue: "<<(void*)v<<" "<<
			iNumPending<<" ? "<<Fifo<T>::iBufferSize);

		if(iNumPending == Fifo<T>::iBufferSize)
			return false;

		*pCursor++ = v;

		if(pCursor >= Fifo<T>::pGuard){
			pCursor = Fifo<T>::tabBuffer;
		}


		if(++iNumPending == Fifo<T>::CChunkSize){
			prepareWrite();		
		}

		return true;
	}

	void flush(){
		prepareWrite();
	}

protected:

	T* pCursor;
	T* pIOWriteHead;
	size_t iNumPending;

	void prepareWrite(){

		if(bInProgress)
			return;

		size_t iRequestSize = iNumPending;	
		if(pIOWriteHead + iRequestSize >= Fifo<T>::pGuard){
			iRequestSize = Fifo<T>::pGuard - pIOWriteHead;
		}

		if(iRequestSize == 0)
			return;

		IA20_LOG(true, "Writing: "<<iRequestSize);
		

		URing::IO::WriteHandler::iovec.iov_base = pIOWriteHead;
		URing::IO::WriteHandler::iovec.iov_len  = iRequestSize * sizeof(T);
		URing::IO::WriteHandler::prepare();
		bInProgress = true;
	}

	virtual void handleWrite(off_t iDataLen){
		
		bInProgress = false;
		pIOWriteHead += iDataLen / sizeof(T);
		iNumPending  -= iDataLen / sizeof(T);

		IA20_LOG(true, "Written: "<<iDataLen<<", IO offset: "
			<<(pIOWriteHead-Fifo<T>::tabBuffer)<<", iNumPending: "<<iNumPending);

		if(pIOWriteHead >= Fifo<T>::pGuard){
			pIOWriteHead = Fifo<T>::tabBuffer;
		}

		if(iNumPending > 0) // TODO wait for chunk ??
			prepareWrite();
	}

	bool bInProgress;
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_FifoOutput_H_ */
