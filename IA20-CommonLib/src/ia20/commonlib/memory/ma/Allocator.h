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

#ifndef _IA20_Allocator_H_
#define _IA20_Allocator_H_

#include <cstddef>
#include <iostream>

namespace IA20 {

class Allocator {
public:

	virtual void* allocate(size_t n)  = 0;
	virtual void free(const void* p)  = 0;
	virtual bool check(const void* p) = 0;
  virtual void trim() = 0;


	inline bool hasCheckPointersOnReturn()const{
		return bCheckPointersOnReturn;
	}

	virtual void printToStream(std::ostream& os)=0;


	virtual ~Allocator()throw() {};

protected:

	Allocator():bCheckPointersOnReturn(true){};

	bool bCheckPointersOnReturn;
};

typedef ::IA20::Allocator* (*PtrFunAllocator)();

template<class T>
	class STLAllocator{

	 public:
		STLAllocator(Allocator* a):a(a){};

		/*
		 * STL pattern starts here.
		 */
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;

		pointer allocate(size_type n, const void * = 0) {
			return (T*) (a->allocate(sizeof(T) * n));
		}

		void deallocate(void* p, size_type) {
			if (!p)
				return;
			a->free(p);
		}

		pointer address(reference x) const {
			return &x;
		}
		const_pointer address(const_reference x) const {
			return &x;
		}

		//STLAllocator<T>& operator=(const STLAllocator<T>& o) {
		//	a=o.a;
		//	return *this;
		//}

		void construct(pointer p, const T& val) {
			new ((T*) p) T(val);
		}
		void destroy(pointer p) {
			if (p)
				p->~T();
		}

		size_type max_size() const {
			return size_t(-1);
		}

		template<class U>
		bool operator==(const STLAllocator<U>&o) const throw () {
			return a==o.a;
		}

		template<class U>
		bool operator!=(const STLAllocator<U>&o) const throw () {
			return a!=o.a;
		}

		template<class U>
		struct rebind {
			typedef STLAllocator<U> other;
		};

		template<class U>
			STLAllocator(const STLAllocator<U>& o):a(o.a){}

		template<class U>
		STLAllocator& operator=(const STLAllocator<U>& o) {
			a=o.a;
			return *this;
		}

		Allocator *a;
	};

template<class T, PtrFunAllocator ff>
class AllocatorFactory {

public:
	/*
	 * STL pattern starts here.
	 */
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	AllocatorFactory() { (*ff)();	}
	AllocatorFactory(const AllocatorFactory&) {	}

	pointer allocate(size_type n, const void * = 0) {
		return (T*) ((*ff)()->allocate(sizeof(T) * n));
	}

	void deallocate(void* p, size_type) {
		if (!p)
			return;
		(*ff)()->free(p);
	}

	pointer address(reference x) const {
		return &x;
	}
	const_pointer address(const_reference x) const {
		return &x;
	}
	AllocatorFactory<T, ff>& operator=(const AllocatorFactory&) {
		return *this;
	}
	void construct(pointer p, const T& val) {
		new ((T*) p) T(val);
	}
	void destroy(pointer p) {
		if (p)
			p->~T();
	}
	size_type max_size() const {
		return size_t(-1);
	}

#ifdef __GNUC__
	template<class U>
	bool operator==(const AllocatorFactory<U, ff>&) const throw () {
		return true;
	}

	template<class U>
	bool operator!=(const AllocatorFactory<U, ff>&) const throw () {
		return false;
	}

	template<class U>
	struct rebind {
		typedef AllocatorFactory<U, ff> other;
	};

	template<class U>
	AllocatorFactory(const AllocatorFactory<U, ff>&) {
	}

	template<class U>
	AllocatorFactory& operator=(const AllocatorFactory<U, ff>&) {
		return *this;
	}
#endif
};

}

#endif /*_IA20_Allocator_H_*/
