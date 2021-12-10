/*
 * File: DataObjectArray.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "DataObjectArray.h"
#include "DataObjectContainer.h"
#include <ia20/dm/memory/MemoryManager.h>

namespace IA20 {
namespace DM {

/*************************************************************************/
DataObjectArray::DataObjectArray(const Type* pType, DataObject *pParent):
  DataObject(pType, pParent){
	IA20_TRACER;
}
/*************************************************************************/
DataObjectArray::~DataObjectArray() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
