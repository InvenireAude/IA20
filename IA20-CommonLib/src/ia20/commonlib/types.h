/*
 * File: IA20-CommonLib/src/commonlib/types.h
 *
 * Copyright (C) 2021, Albert Krzymowski
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

#ifndef IA20TYPES_H_
#define IA20TYPES_H_

//using namespace std;
#include <string>
#include <sstream>
#include <list>

#include "memory/mm/MemoryManager.h"
#include "memory/ma/Allocator.h"

#ifdef __GNUC__
#define  IA20_std_unordered_map std::unordered_map
#define  IA20_std_hash          std::hash
#else
#define  IA20_std_unordered_map std::tr1::unordered_map
#define  IA20_std_hash          std::tr1::hash
#endif

namespace IA20{

	typedef double Float;
  typedef long   Long;

	typedef std::basic_string< char,
		std::char_traits<char>,
    AllocatorFactory< char, &MemoryManager::GetAllocator >
		> String;

	typedef std::basic_string< wchar_t,
		std::char_traits<wchar_t>,
		AllocatorFactory< wchar_t, &MemoryManager::GetAllocator >
		> WString;

	typedef std::basic_stringstream< char,
									 std::char_traits<char>,
									AllocatorFactory<char, &MemoryManager::GetAllocator> > StringStream;

	typedef std::list<String, AllocatorFactory<String, &MemoryManager::GetAllocator> >
			StringList;

  typedef std::list< std::pair<String,String> ,
      AllocatorFactory<std::pair<String,String>, &MemoryManager::GetAllocator>
   >
			StringPairList;

};

#endif /*IA20TYPES_H_*/
