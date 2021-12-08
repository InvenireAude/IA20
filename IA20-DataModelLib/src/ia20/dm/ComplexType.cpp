/*
 * File: ComplexType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ComplexType.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
ComplexType::ComplexType(Kind iKind):
  Type(iKind){
	IA20_TRACER;
}

/*************************************************************************/
ComplexType::~ComplexType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
