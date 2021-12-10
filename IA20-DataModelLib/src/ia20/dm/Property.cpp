/*
 * File: Property.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Property.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
Property::Property(const Type* pType, const String& strName, unsigned int iIdx):
  pType(pType),
  strName(strName),
  iIdx(iIdx){
	IA20_TRACER;
}

/*************************************************************************/
Property::~Property() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
