/*
 * File: ContextOutputDirect.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ContextOutputDirect.h"


namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
ContextOutputDirect::ContextOutputDirect(uint8_t *pMessage):
	sbl(pMessage),
	reader(sbl){
	IA20_TRACER;  
}
/*************************************************************************/
ContextOutputDirect::~ContextOutputDirect() throw(){
	IA20_TRACER;
}
/*************************************************************************/
bool ContextOutputDirect::next(){
	reader.getNext();
	return reader.hasData();
}
/*************************************************************************/
uint8_t *ContextOutputDirect::getData(){
	return reader.getData();
}
/*************************************************************************/
uint32_t ContextOutputDirect::getDataLength()const{
	return reader.getLength();
}
/*************************************************************************/
}
}
}
