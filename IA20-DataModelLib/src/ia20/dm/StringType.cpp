/*
 * File: StringType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "StringType.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
StringType::StringType():
SimpleType(Type::CStringType){
	IA20_TRACER;
}

/*************************************************************************/
StringType::~StringType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
