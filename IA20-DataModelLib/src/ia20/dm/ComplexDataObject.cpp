/*
 * File: ComplexDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ComplexDataObject.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
ComplexDataObject::ComplexDataObject(const Type* pType, DataObject *pParent):
  DataObject(pType, pParent){
	IA20_TRACER;
}

/*************************************************************************/
ComplexDataObject::~ComplexDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
