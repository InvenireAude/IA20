/*
 * File: StreamBufferList.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Memory_StreamBufferList_H_
#define _IA20_IOT_Memory_StreamBufferList_H_

#include <ia20/commonlib/commonlib.h>


namespace IA20 {
namespace IOT {
namespace Memory {

class SharableMemoryPool;

/*************************************************************************/
/** The StreamBufferList class.
 *
 */
//TODO template ?
class StreamBufferList {

public:	

	typedef	int32_t OffsetNextType;
	typedef	uint32_t DataLengthType;
	

protected:

	struct Chunk {

		static  DataLengthType CMaxChunkSize;

		OffsetNextType iOffsetNext;
		DataLengthType iDataLength;
		DataLengthType iChunkSize;
		int32_t	       _pad;

		Chunk(DataLengthType iChunkSize):
			iOffsetNext(0),
			iDataLength(0),
			iChunkSize(iChunkSize){};

		inline void setNext(Chunk *pChunk){		
			// compress, TODO addjust to ShareableMemoryPool rounding/alignment.
			iOffsetNext =  pChunk ? ((uint8_t*)pChunk - (uint8_t*)this) >> 3 : 0;		
		}

		inline Chunk* getNext(){
			return reinterpret_cast<Chunk*>(reinterpret_cast<uint8_t*>(this) + (iOffsetNext << 3));
		}

		inline const Chunk* getNext()const{
			return const_cast<Chunk*>(this)->getNext();
		}


		inline bool isLast()const{
			return !iOffsetNext;
		}

		inline uint8_t* getDataStart()const{
			return reinterpret_cast<uint8_t*>((const_cast<Chunk*>(this)))+sizeof(Chunk);
		}

		friend
		std::ostream& operator<<(std::ostream& os,const Chunk& c){
			os<<"Chunk :"<<(void*)&c<<" ["<<c.iOffsetNext<<":"<<(int)c.iOffsetNext*(int)sizeof(Chunk)<<","<<c.iDataLength<<","<<c.iDataLength<<"]";
			return os;
		}
	};

public:

	StreamBufferList(SharableMemoryPool* pMemoryPool, void* pOnwerAddress);
	StreamBufferList(void *pFromMemory);
	
	~StreamBufferList() throw();

	class Reader {
	public:
		inline Reader(const StreamBufferList& sbl):
			pChunk(sbl.pHead),bFinished(false){}

		bool getNext(uint8_t* &refPtrData, DataLengthType& iDataLength);
		
		protected:
			const Chunk *pChunk;
			bool   bFinished;
	};

	class Writer{
	public:
		inline Writer(StreamBufferList& sbl):
			sbl(sbl),
			pChunk(sbl.pHead),
			pCursor(NULL),
			iAvailableLength(0){}

	 	void next(DataLengthType iMinDataLength);
	
		inline void addData(DataLengthType iNewDataLength){
			pChunk->iDataLength += iNewDataLength;
		}

		inline DataLengthType getAvailableLength()const{
			return iAvailableLength;
		}

		inline uint8_t* getCursor()const{
			return pCursor;
		}

		protected:
			Chunk *pChunk;
			StreamBufferList& sbl;
			DataLengthType  iAvailableLength;
			uint8_t*        pCursor;
	};


	inline Chunk* getHead(){
		IA20_CHECK_IF_NULL(pHead);
		return pHead;
	}

protected:
	Chunk *pHead;
	SharableMemoryPool* pMemoryPool;

	Chunk *allocateChunk(void* pOnwerAddress)const;
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Memory_StreamBufferList_H_ */
