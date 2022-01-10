/*
 * File: ConfigException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ConfigException.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
ConfigException::ConfigException(){
	IA20_TRACER;
}
/*************************************************************************/
ConfigException::ConfigException(const String& strInfo):
  IOT::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
ConfigException::~ConfigException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* ConfigException::getName(){
	IA20_TRACER;
	return "ConfigException";
}
/*************************************************************************/
}
}
