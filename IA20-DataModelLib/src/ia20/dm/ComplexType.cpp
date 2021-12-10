/*
 * File: ComplexType.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "ComplexType.h"

#include <ia20/dm/memory/MemoryManager.h>

#include "ComplexDataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
ComplexType::ComplexType(const String& strName, const Type* pParent):
  Type(Type::CDataObjectType, strName),
  iPropertyOffset(-1){
	IA20_TRACER;

  if(!pParent){
    iPropertyOffset = 0;
    this->pParent = NULL;
   }else{
    this->pParent = pParent->asComplexType();
   }
}
/*************************************************************************/
ComplexType::~ComplexType() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const ComplexType* ComplexType::asComplexType()const{
   return this;
}
/*************************************************************************/
void ComplexType::defineProperty(const Type* pType, const String& strName){
	IA20_TRACER;

  if(iPropertyOffset == -1 && pParent){
    const PropertiesArray* pParentProperties = this->pParent->getProperties();
    iPropertyOffset = pParentProperties->size();
    IA20_LOG(true, "ComplexType: "<<strName<<", parent size: "<<iPropertyOffset);
    tabProperties.insert(tabProperties.end(),pParentProperties->begin(), pParentProperties->end());
  }

  unsigned int iIdx = iPropertyOffset + tabLocalProperties.size();

  std::unique_ptr<Property> ptrProperty(new Property(pType, strName, iIdx));

  tabLocalProperties.push_back(std::move(ptrProperty));
  tabProperties.push_back(tabLocalProperties.back().get());
}
/*************************************************************************/
DataObject* ComplexType::create(DataObject *pParent)const{

	IA20_TRACER;

  DataObject *pResult = NULL;

  pResult = Memory::MemoryManager::AllocateLocally<ComplexDataObject>(pParent);
  new(pResult) ComplexDataObject(this, pParent);

  return pResult;
}
/*************************************************************************/
}
}
