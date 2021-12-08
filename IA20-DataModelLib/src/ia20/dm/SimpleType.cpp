/*
 * File: SimpleType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "SimpleType.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
SimpleType::SimpleType(Kind iKind):
 Type(iKind){
	IA20_TRACER;
}

/*************************************************************************/
SimpleType::~SimpleType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
