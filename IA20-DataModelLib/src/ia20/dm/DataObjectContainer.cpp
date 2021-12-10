/*
 * File: DataObjectContainer.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "DataObjectContainer.h"

#include <ia20/dm/exception/ConversionException.h>

#include <ia20/dm/memory/MemoryManager.h>


namespace IA20 {
namespace DM {

/*************************************************************************/
DataObjectContainer::DataObjectContainer(){
	IA20_TRACER;
  tabDataObjects = reinterpret_cast<DataObject**>(Memory::MemoryManager::AllocateLocally(this, sizeof(DataObject*) * 1000));
  iSize = 0;
}
/*************************************************************************/
DataObjectContainer::~DataObjectContainer() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}

