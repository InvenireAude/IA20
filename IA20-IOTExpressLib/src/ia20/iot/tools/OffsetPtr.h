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

template<uint8_t  iBitShift = 0,
         typename HolderType = int32_t>
class OffsetPtr {
public:

  OffsetPtr():ptr(0){};
  OffsetPtr(const OffsetPtr& o) = delete;
	
  template<typename T>
  inline void set(T* p){
    int64_t v = reinterpret_cast<int64_t>(p) - reinterpret_cast<int64_t>(this);
    ptr = v >> iBitShift;
    //IA20_LOG(true, "Set: ..."<<(int)ptr<<" v="<<v);
  }

  template<typename T>
  inline T* get()const{
    if(!ptr)
      return NULL;
    uint64_t v = reinterpret_cast<int64_t>(this) + (static_cast<int64_t>(ptr) << iBitShift);
    return reinterpret_cast<T*>(v);
  }

  inline bool operator!()const{
    return ptr == 0;
  }

  friend
  std::ostream& operator<<(std::ostream& os, const OffsetPtr& o){
    //<<(void*) nice hex ;)
    os<<"["<<(void*)&o<<":"<<(void*)(long)o.ptr<<"->"<<(void*)o.get<void>()<<"]";
    return os;
  }

protected:
  HolderType ptr;
};
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_OffsetPtr_H_ */

