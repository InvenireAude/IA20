/*
 * File: OffsetPtr.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_OffsetPtr_H_
#define _IA20_IOT_Tools_OffsetPtr_H_

#include <ia20/commonlib/commonlib.h>

#include <memory>

namespace IA20 {
namespace IOT {
namespace Tools {

/*************************************************************************/
/** The OffsetPtr class.
 *
 */

template<uint8_t  iBitShift = 4,
         typename HolderType = int32_t>
class OffsetPtr {
public:

  OffsetPtr():ptr(0){};
  OffsetPtr(const OffsetPtr& o) = delete;
	
  template<typename T>
  inline void set(T* p){
    intptr_t v = reinterpret_cast<intptr_t>(p) - reinterpret_cast<intptr_t>(this);
    ptr = v >> iBitShift;
  }

  template<typename T>
  inline T* get(){
    if(!ptr)
      return NULL;
    intptr_t v = reinterpret_cast<intptr_t>(this) + (reinterpret_cast<intptr_t>(ptr) << iBitShift);
    return reinterpret_cast<T*>(v);
  }

protected:
  HolderType ptr;
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_OffsetPtr_H_ */

