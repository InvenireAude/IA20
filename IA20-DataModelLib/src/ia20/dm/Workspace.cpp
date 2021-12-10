/*
 * File: Workspace.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Workspace.h"
#include "DataObjectContainer.h"
#include <ia20/dm/memory/MemoryManager.h>

namespace IA20 {
namespace DM {

/*************************************************************************/
Workspace::Workspace():
  DataObject(NULL, NULL){
	IA20_TRACER;
}
/*************************************************************************/
Workspace::~Workspace() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
