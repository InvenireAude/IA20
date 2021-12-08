/*
 * File: IntegerType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "IntegerType.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
IntegerType::IntegerType():
  SimpleType(Type::Kind::CIntegerType){
	IA20_TRACER;
}

/*************************************************************************/
IntegerType::~IntegerType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
