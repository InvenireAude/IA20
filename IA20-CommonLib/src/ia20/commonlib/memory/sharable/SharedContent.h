/*
 * File: IA20-CommonLib/src/commonlib/memory/sharable/SharedContent.h
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef _IA20_SharedContent_H_
#define _IA20_SharedContent_H_

#include "../memory.h"
#include "SharedMemoryFile.h"

namespace IA20{

//============================================================================

template<class T>
class SharedContent {

public:

	typedef SharedMemoryFile::Descriptor Descriptor;

	T* get(){ return pContent; };

	virtual ~SharedContent(){

		if(ptrSHMFile->getDescriptor().getCloseMode() ==
			SharedMemoryFile::Descriptor::CM_DESTROY)
		  pContent->~T();
	}

	template<typename ... Ts>
	SharedContent(const Descriptor& refDescriptor, Ts... args){
		init(refDescriptor);
		if(!ptrSHMFile->alreadyExisted())
			new (pContent)T(&args...);
	}



protected:

	std::unique_ptr<SharedMemoryFile> ptrSHMFile;
	T *pContent;

	void init(const Descriptor& refDescriptor){

		IA20_TRACER;

		Descriptor aDescriptor(refDescriptor);
		aDescriptor.setSize(sizeof(T));
		ptrSHMFile = SharedMemoryFile(aDescriptor);
		pContent = (T*)(ptrSHMFile->getAddress());
	}

};

//============================================================================

};

#endif /*_IA20_Factory_H_*/
