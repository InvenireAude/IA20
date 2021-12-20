/*
 * File: ItemNotFoundException.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ItemNotFoundException.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
ItemNotFoundException::ItemNotFoundException(){
	IA20_TRACER;
}
/*************************************************************************/
ItemNotFoundException::ItemNotFoundException(const String& strInfo):
  IOT::Exception(strInfo){
	IA20_TRACER;
}
/*************************************************************************/
ItemNotFoundException::~ItemNotFoundException() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const char* ItemNotFoundException::getName(){
	IA20_TRACER;
	return "ItemNotFoundException";
}
/*************************************************************************/
}
}
