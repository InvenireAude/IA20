/*
 * File: IdentifiedByHandle.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_IdentifiedByHandle_H_
#define _IA20_IOT_Tools_IdentifiedByHandle_H_

#include <ia20/commonlib/commonlib.h>

#include <inttypes.h>
  
namespace IA20 {
namespace IOT {
namespace Tools {

class HandleSetter;
/*************************************************************************/
/** The IdentifiedByHandle class.
 *
 */
class IdentifiedByHandle {
public:

	typedef uint64_t HandleType;

	inline HandleType getHandle()const{
		return mHandle;		
	}

	static const HandleType CUnset = ~0L;
protected:

	IdentifiedByHandle(HandleType aHandle = CUnset):mHandle(aHandle){}

	HandleType mHandle;

	friend HandleSetter;
};

class HandleSetter{
	protected:

	inline void	setHandle(IdentifiedByHandle& ih, IdentifiedByHandle::HandleType aHandle){
		ih.mHandle = aHandle; 
	}
};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Tools_IdentifiedByHandle_H_ */
