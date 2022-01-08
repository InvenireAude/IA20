/*
 * File: ActivityStore.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ActivityStore.h"


namespace IA20 {
namespace IOT {

/*************************************************************************/
ActivityStore::ActivityStore(
	IndexType     iSize
):
iSize(iSize),
iHead(0),
tActivites(NULL),
iNumActivites(0){
	IA20_TRACER;
	tActivites = static_cast<Activity*>(std::aligned_alloc(sizeof(Activity)*iSize, alignof(Activity)));
}
/*************************************************************************/
ActivityStore::~ActivityStore() throw(){
	IA20_TRACER;

	if(tActivites)
		std::free(tActivites);
}
/*************************************************************************/
}
}
