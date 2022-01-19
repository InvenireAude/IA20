/*
 * File: Fifo.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SYS_Fifo_H_
#define _IA20_IOT_Tools_SYS_Fifo_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/uring/uring.h>

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SYS {

/*************************************************************************/
/** The Fifo class.
 *
 */
template<class T>
class Fifo 
	//public IA20::URing::IO::CloseHandler
	 {

public:

	Fifo(size_t iBufferSize):
		iBufferSize(iBufferSize){
		tabBuffer = (T*)std::aligned_alloc(sizeof(T), sizeof(T) * iBufferSize);
		pGuard = tabBuffer + iBufferSize;
	}

	virtual ~Fifo(){
		std::free(tabBuffer);
	}

protected:

	static const size_t CChunkSize = PAGE_SIZE / sizeof(T);

	size_t iBufferSize;
	T*     tabBuffer;
	T*     pGuard;
};

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SYS_Fifo_H_ */
