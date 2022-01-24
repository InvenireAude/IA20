/*
 * File: ContextOutputShared.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ContextOutputShared.h"


namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
ContextOutputShared::ContextOutputShared(uint8_t *pMessage, uint8_t* pPayLoad):
	iReaderIdx(1),
	sbl1(pMessage),
	reader1(sbl1),
	sbl2(pPayLoad),
	reader2(sbl2){
	IA20_TRACER;  
	
	/* optimize the first packet and try to conatenate data*/

	if(sbl1.getAvailableSpace() > reader2.getLength()){
		Memory::StreamBufferList::Writer writer(sbl1);
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Concatenate content: "<<reader1.getLength()<<" + "<<reader2.getLength());
		writer.write(reader2.getData(), reader2.getLength() );
		new (&reader1)Memory::StreamBufferList::Reader(sbl1);
	}

}
/*************************************************************************/
ContextOutputShared::~ContextOutputShared() throw(){
	IA20_TRACER;

}
/*************************************************************************/
bool ContextOutputShared::next(){
	
	if(iReaderIdx == 1){
		iReaderIdx = 2;
	}else{
		reader2.getNext();
	}

	return reader2.hasData();
}
/*************************************************************************/
uint8_t  *ContextOutputShared::getData(){
	if(iReaderIdx == 1){
		return reader1.getData();
	}else{
		return reader2.getData();
	}
}
/*************************************************************************/
Memory::StreamBufferList::DataLengthType ContextOutputShared::getDataLength()const{

	if(iReaderIdx == 1){
		return reader1.getLength();
	}else{
		return reader2.getLength();
	}
}
/*************************************************************************/
}
}
}
