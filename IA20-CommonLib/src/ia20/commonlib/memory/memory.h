/*
 * File: IA20-CommonLib/src/commonlib/memory/memory.h
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

#ifndef IA20MEMORY_H_
#define IA20MEMORY_H_

#include "mm/MemoryManager.h"
#include "../exception/MemoryException.h"
#include "../exception/InternalException.h"

#include "errno.h"

#include <memory>

#define IA20_CHECK_IF_NULL(p)       \
{                                                                       \
        if(!p){                                                  \
        	IA20_THROW(IA20::InternalException("null pointer!"))          \
        }                                                               \
}

//TODO (M) Check entries if know pointer and test
#define IA20_CHECK_IF_VALID(p)       \
{                                                                       \
        if(!p){                                                  \
        	IA20_THROW(IA20::InternalException("null pointer!"))          \
        }                                                               \
}

#define IA20_DFT_FACTORY ::IA20::MemoryManagerFactory



inline void* operator new(size_t size){
  return ::IA20::MemoryManager::GetInstance()->allocate(size);
}

inline void* operator new[](size_t size){
  return ::IA20::MemoryManager::GetInstance()->allocate(size);
}

inline void operator delete(void* p){
  return ::IA20::MemoryManager::GetInstance()->free(p);
}

inline void operator delete[](void* p){
  return ::IA20::MemoryManager::GetInstance()->free(p);
}

#endif /*IA20MEMORY_H_*/
