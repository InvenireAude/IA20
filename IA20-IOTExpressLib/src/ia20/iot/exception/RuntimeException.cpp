/*
 * File: RuntimeException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "RuntimeException.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
RuntimeException::RuntimeException(){
	IA20_TRACER;
}
/*************************************************************************/
RuntimeException::RuntimeException(const String& strInfo):
  IOT::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
RuntimeException::~RuntimeException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* RuntimeException::getName(){
	IA20_TRACER;
	return "RuntimeException";
}
/*************************************************************************/
}
}
