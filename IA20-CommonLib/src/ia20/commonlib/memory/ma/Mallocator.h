/*
 * File: IA20-CommonLib/src/commonlib/memory/ma/Allocator.h
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
/* ChangeLog:
 *
 */

#ifndef _IA20_Mallocator_H_
#define _IA20_Mallocator_H_

#include <cstddef>
#include <iostream>
#include <map>
#include <unordered_map>

/*
 * This should be used (and 'bare' types defined bellow) when MemoryManagers can interfere, e.g.
 * when logging or initializing some global variables.
 */

namespace IA20 {
template <class T>
struct Mallocator
{
    typedef T value_type;
    Mallocator() noexcept {}

    template<class U> Mallocator(const Mallocator<U>&) noexcept {}

    template<class U> bool operator==(const Mallocator<U>&) const noexcept{
        return true;
    }

    template<class U> bool operator!=(const Mallocator<U>&) const noexcept{
        return false;
    }

    T* allocate(const size_t n) const;
    void deallocate(T* const p, size_t) const noexcept;
};

template <class T>
T* Mallocator<T>::allocate(const size_t n) const
{
    if (n == 0){
        return nullptr;
    }

    if (n > static_cast<size_t>(-1) / sizeof(T)){
        throw std::bad_array_new_length();
    }

    void* const pv = malloc(n * sizeof(T));

    if (!pv) {
      throw std::bad_alloc();
    }

    return static_cast<T*>(pv);
}

template<class T>
void Mallocator<T>::deallocate(T * const p, size_t) const noexcept{
    free(p);
}


typedef std::basic_string<char, std::char_traits<char>, Mallocator<char> > BareString;
typedef std::basic_stringstream< char,
									 std::char_traits<char>,
									 Mallocator<char> > BareStringStream;

template<class K, class V>
class BareMap : public std::map<K, V, std::less<K>, Mallocator<std::pair<const K, V> > >{};

template<class K, class V>
class BareHashMap : public std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, Mallocator<std::pair<const K, V> > >{};

}

#endif /*_IA20_Allocator_H_*/
