/*
 * File: ComplexDataObject.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include <ia20/dm/memory/MemoryManager.h>

#include "ComplexDataObject.h"
#include "ComplexType.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
ComplexDataObject::ComplexDataObject(const ComplexType* pType, DataObject *pParent):
  DataObject(pType, pParent),
  pComplexType(pType){
	IA20_TRACER;

  tabDataObjects = reinterpret_cast<DataObject**>(Memory::MemoryManager::AllocateLocally(this, sizeof(DataObject*) *
    pComplexType->getNumProperties()));

}

/*************************************************************************/
ComplexDataObject::~ComplexDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void ComplexDataObject::createProperty(unsigned int iIdx){
  IA20_TRACER;
// IA20_LOG(true, "New property ["<<iIdx<<"]: "<<mValue.pType->asComplexType()->getProperty(iIdx)->getType()->getName());
  tabDataObjects[iIdx] = pComplexType->getProperty(iIdx)->getType()->create(this);
}
/*************************************************************************/
void ComplexDataObject::setProperty(unsigned int iIdx, DataObject* pDataObject){
  IA20_TRACER;
  tabDataObjects[iIdx] = pDataObject;
}
/*************************************************************************/
void ComplexDataObject::setProperty(const String& strName, DataObject* pDataObject){
  IA20_TRACER;
  tabDataObjects[pComplexType->getProperty(strName)->getIdx()] = pDataObject;
}
/*************************************************************************/
DataObject* ComplexDataObject::getProperty(unsigned int iIdx)const{
  IA20_TRACER;
  return tabDataObjects[iIdx];
}
/*************************************************************************/
DataObject* ComplexDataObject::getProperty(const String& strName)const{
  IA20_TRACER;
  return tabDataObjects[pComplexType->getProperty(strName)->getIdx()];
}
/*************************************************************************/
}
}
