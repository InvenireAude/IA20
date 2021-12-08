/*
 * File: MemoryManager.h
 * 
 * Copyright (C) 2021, Albert Krzymowski
 * 

 */


#ifndef _IA20_DM_Memory_MemoryManager_H_
#define _IA20_DM_Memory_MemoryManager_H_

#include <ia20/commonlib/commonlib.h>
using namespace IA20;

namespace IA20 {
namespace DM {
namespace Memory {

/*************************************************************************/
/** The MemoryManager class.
 *
 */
class MemoryManager {
public:


	virtual ~MemoryManager() throw();

	MemoryManager();
};

/*************************************************************************/
}
}
}

#endif /* _IA20_DM_Memory_MemoryManager_H_ */
