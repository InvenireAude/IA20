/*
 * File: StreamBufferList.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Memory_StreamBufferList_H_
#define _IA20_IOT_Memory_StreamBufferList_H_

#include <iostream>

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/OffsetPtr.h>
#include <ia20/iot/tools/StringRef.h>
#include <ia20/iot/logger/LogLevel.h>

#include <string.h>

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

	typedef	Tools::OffsetPtr<3> OffsetNextType;
	typedef	uint32_t DataLengthType;


protected:

	struct Chunk {

		static  DataLengthType CMaxChunkSize;

		OffsetNextType offsetNext;
		DataLengthType iDataLength;
		DataLengthType iChunkSize;
		int32_t	       _pad;

		Chunk(DataLengthType iChunkSize):
			iDataLength(0),
			iChunkSize(iChunkSize){};

		inline void setNext(Chunk *pChunk){
			offsetNext.set(pChunk);
		}

		inline Chunk* getNext(){
			return offsetNext.get<Chunk>();
		}

		inline const Chunk* getNext()const{
			return const_cast<Chunk*>(this)->getNext();
		}

		inline bool isLast()const{
			return !offsetNext;
		}

		inline uint8_t* getDataStart()const{
			return reinterpret_cast<uint8_t*>((const_cast<Chunk*>(this)))+sizeof(Chunk);
		}

		friend
		std::ostream& operator<<(std::ostream& os,const Chunk& c){
			os<<"Chunk :"<<(void*)&c<<" {"<<c.offsetNext<<" "<<c.iDataLength<<","<<c.iDataLength<<"}";
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
			pChunk(sbl.pHead),
			bFinished(false),
			pData(pChunk->getDataStart()),
			iConsumedBytes(0),
			iDataLength(pChunk->iDataLength){
				IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Starting at: "<<(void*)pData<<", len: "<<iDataLength);
			}

		void getNext();

		inline uint8_t* getData()const{
			return pData;
		}

		inline DataLengthType getLength()const{
			return iDataLength;
		}

		inline void advance(DataLengthType iStep){

			if(iStep < iDataLength){
				iDataLength -= iStep;
				pData += iStep;
				iConsumedBytes += iStep;
			}else if(iStep == iDataLength){
				 getNext();
				 iConsumedBytes += iStep;
			}else{
				IA20_THROW(InternalException("advance(iStep > iDataLenght)"));
			}

			IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Consumed Bytes: "<<iConsumedBytes);
		}

		inline DataLengthType copy(uint8_t* pDst, DataLengthType iBytesToCopy = 10000){

			DataLengthType iLeft = iBytesToCopy;

			while(iLeft > 0 && hasData()){

				DataLengthType iStepSize = iLeft;

				if(iStepSize > iDataLength){
					iStepSize = iDataLength;
				}

				memcpy(pDst, pData, iStepSize);

				iLeft -= iStepSize;
				iDataLength -= iStepSize;
				iConsumedBytes += iStepSize;

				if(iDataLength == 0){
					getNext();
				}else{
					pData += iStepSize;
				}

			}

			IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Consumed Bytes: "<<iConsumedBytes);
			return iBytesToCopy - iLeft;
		}

		inline bool hasData()const{
			return !(pChunk->isLast() && iDataLength == 0);
		}

		inline uint8_t readByte(){

			IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Reading at: "<<(void*)pData<<", v:"<<(int)*pData<<", len: "<<iDataLength);

			if(!iDataLength){
				getData();
			}

			if(iDataLength == 0)
				IA20_THROW(EndOfDataException("StreamBuffer is empty!!!"));

			iDataLength--;
			iConsumedBytes++;
			IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Consumed Bytes: "<<iConsumedBytes);

			return *pData++;
		}

			inline DataLengthType getConsumedBytes()const{
				return iConsumedBytes;
			}

		protected:
			const Chunk *pChunk;
			bool   bFinished;

			uint8_t* pData;
			DataLengthType iDataLength;
			DataLengthType iConsumedBytes;;
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
			iAvailableLength    -= iNewDataLength; // TODO remove this redundant value
			pCursor             += iNewDataLength;
		}

		inline DataLengthType getAvailableLength()const{
			return iAvailableLength;
		}

		inline uint8_t* getCursor()const{
			return pCursor;
		}

		inline void write(const uint8_t* pNewData, DataLengthType iNewDataLength){

			while(iNewDataLength > 0){

				next(sizeof(Chunk));

				DataLengthType iChunkLen =
					iNewDataLength <= iAvailableLength ? iNewDataLength : iAvailableLength;

       			 IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "pCursor:"<<(void*)pCursor<<", "<<iChunkLen);

				memcpy(pCursor, pNewData, iChunkLen);
				addData(iChunkLen);
				pNewData       += iChunkLen;
				iNewDataLength -= iChunkLen;
			}
		}

		inline void writeByte(const uint8_t iValue){

				if(iAvailableLength == 0){
					next(sizeof(Chunk));
				}
				
				*pCursor = iValue;
				addData(1);
		}

		inline void writeTwoBytes(const uint16_t iValue){

				if(iAvailableLength <= 1){
					next(sizeof(Chunk));
				}
				
				*pCursor       = 0xff  & (iValue >> 8);
				*(pCursor + 1) = 0xff & iValue;

				addData(2);
		}

		inline void write(const Tools::StringRef& strValue){
			writeTwoBytes(strValue.getLength());
			write(strValue.getData(), strValue.getLength());
		}
		protected:
			Chunk *pChunk;
			StreamBufferList& sbl;
			DataLengthType  iAvailableLength;
			uint8_t*        pCursor;
	};


	inline uint8_t* getHead()const{
		IA20_CHECK_IF_NULL(pHead);
		return (uint8_t*)pHead;
	}

	inline DataLengthType getAvailableSpace()const{
		return pHead->iChunkSize - pHead->iDataLength;
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
