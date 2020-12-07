/*
 * File: IA20-CommonLib/src/commonlib/tools/Buffer.h
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
#ifndef _IA20_Buffer_H_
#define _IA20_Buffer_H_

#include "../types.h"

namespace IA20 {

/*************************************************************************/

class Buffer{
	public:
		Buffer(size_t iSize=0, Allocator* ma=MemoryManager::GetAllocator());
		virtual	~Buffer();

		void reserve(size_t iSize);
		void resize(size_t iSize);

		size_t getSize()const;

		template<typename T>
		T* getBuffer(){
			return (T*)getBuffer<void>();
		}

		template<typename T>
		const T* getBuffer()const{
			return (T*)const_cast<Buffer*>(this)->getBuffer<void>();
		}

		template<typename T>
		operator T*(){ return getBuffer<T>(); }

		template<typename T>
		Buffer& operator =(const T& v){ *(getBuffer<T>())=v;  return *this;}

    void* pass();

	protected:
		void  *pData;
		size_t iSize;

		Allocator* ma;

struct MemHolder {

	MemHolder(void* p, Allocator *ma) :
			p(p),ma(ma) {
	}

	~MemHolder() {
		if(p)
			ma->free(p);
	}

	operator void*(){
		return p;
	}

	void* pass() {
		void* tmp = p;
		p = 0;
		return tmp;
	}

	void* p;
	Allocator *ma;
};

};

//TODO repair this !
template<>
void* Buffer::getBuffer<void>();
/*************************************************************************/
}

#endif /* _IA20_EnvTools_H_ */
