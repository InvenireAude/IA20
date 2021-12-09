/*
 * File: SimpleDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "SimpleDataObject.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
SimpleDataObject::SimpleDataObject(const Type* pType, DataObject *pParent):
  DataObject(pType, pParent){
	IA20_TRACER;
}

/*************************************************************************/
SimpleDataObject::~SimpleDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
