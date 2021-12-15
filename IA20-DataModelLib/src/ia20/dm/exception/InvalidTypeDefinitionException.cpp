/*
 * File: InvalidTypeDefinitionException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "InvalidTypeDefinitionException.h"


namespace IA20 {
namespace DM {

/*************************************************************************/
InvalidTypeDefinitionException::InvalidTypeDefinitionException(){
	IA20_TRACER;
}
/*************************************************************************/
InvalidTypeDefinitionException::InvalidTypeDefinitionException(const String& strInfo):
  DM::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
InvalidTypeDefinitionException::~InvalidTypeDefinitionException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* InvalidTypeDefinitionException::getName(){
	IA20_TRACER;
	return "InvalidTypeDefinitionException";
}
/*************************************************************************/
}
}
