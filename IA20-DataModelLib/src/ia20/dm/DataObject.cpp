/*
 * File: DataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "DataObject.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
DataObject::DataObject(DataObject *pParent, const TypedValue& mValue):
  pParent(pParent),
  mValue(mValue){
	IA20_TRACER;
}

/*************************************************************************/
DataObject::~DataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
