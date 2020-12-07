/*
 * File: IA20-CommonLib/src/commonlib/types.h
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

#ifndef IA20TYPES_H_
#define IA20TYPES_H_


#include <string>
#include <sstream>
#include <list>


namespace IA20{

	typedef double Float;
  typedef long   Long;

	typedef std::basic_string< char,
		std::char_traits<char>,
		AllocatorFactory< char, &IA20::MemoryManager::GetAllocator >
		> String;

	typedef std::basic_string< wchar_t,
		std::char_traits<wchar_t>,
		AllocatorFactory< wchar_t, &IA20::MemoryManager::GetAllocator >
		> WString;

	typedef std::basic_stringstream< char,
									 std::char_traits<char>,
									AllocatorFactory< char, &IA20::MemoryManager::GetAllocator > > StringStream;

	typedef std::list<String, AllocatorFactory<String, &IA20::MemoryManager::GetAllocator > >
			StringList;

	typedef std::list< std::pair<String,String> /*, AllocatorFactory<String, &MemoryManager::GetAllocator > */ >
			StringPairList;

};

#endif /*IA20TYPES_H_*/
