/*
 * File: IA20-CommonLib/src/commonlib/tools/Buffer.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
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
#include "Buffer.h"

#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/memory/memory.h>
#include <ia20/commonlib/exception/InternalException.h>

#include <string.h>

namespace IA20 {

/*************************************************************************/
Buffer::Buffer(size_t iSize, Allocator *ma):pData(0),iSize(0),ma(ma){
	IA20_TRACER;
	reserve(iSize);
	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"Buffer, size: "<<(pData ? (void*)getBuffer<char>(): 0));
};
/*************************************************************************/
Buffer::~Buffer(){
	IA20_TRACER;
	reserve(0);
}
/*************************************************************************/
void Buffer::reserve(size_t iSize){

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"Reserve from:"<<this->iSize<<" to "<<iSize<<", data is "<<pData);

	if(iSize == this->iSize)
		return;

	if(pData)
		ma->free(pData);

	if(iSize)
		pData=ma->allocate(iSize);

	this->iSize=iSize;
}
/*************************************************************************/
void Buffer::resize(size_t iSize){

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"Resizing from:"<<this->iSize<<" to "<<iSize);

	if(this->iSize == iSize)
		return;

	if(iSize){

		MemHolder ptrNewData(ma->allocate(iSize),ma);

		if(pData){
			memcpy(ptrNewData,pData,(this->iSize < iSize) ? this->iSize : iSize);
			ma->free(pData);
		}

		this->iSize=iSize;
		pData=ptrNewData.pass();

	}else{
		reserve(0);
	}

	IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(),"pData="<<pData);

}
/*************************************************************************/
size_t Buffer::getSize()const{
	return iSize;
}
/*************************************************************************/
template<>
void *Buffer::getBuffer<void>(){
	IA20_TRACER;
	if(iSize)
		return pData;
	IA20_THROW(InternalException("Buffer is empty."))
}
/*************************************************************************/
void* Buffer::pass(){

  MemHolder mh(pData, ma);

  pData = NULL;

  if(iSize)
    pData = ma->allocate(iSize);

  return mh.pass();
}
/*************************************************************************/

}

