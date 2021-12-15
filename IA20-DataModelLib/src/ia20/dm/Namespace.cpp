/*
 * File: Namespace.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Namespace.h"

#include <ia20/dm/exception/InvalidTypeDefinitionException.h>

namespace IA20 {
namespace DM {

const String& Namespace::CDefaultName("DefaultURI");

/*************************************************************************/
Namespace::Namespace(const String& strName):
  strName(strName),
  hmTypes(20){
	IA20_TRACER;
}
/*************************************************************************/
Namespace::~Namespace() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Type* Namespace::createType(Type::Kind iKind, const String& strName, const Type* pBaseType){
  IA20_TRACER;

  IA20_LOG(true, "Create type:["<<iKind<<"]:["<<this->strName<<"#"<<strName<<"]:parent("<<(pBaseType ? pBaseType->getName() : "NULL")<<")");

  TypeMap::iterator it = hmTypes.find(strName);

  if(it != hmTypes.end()){
    IA20_THROW(InvalidTypeDefinitionException("Type already exists: [")<<strName<<"] in namespace: ["<<
      strName<<"]");
  }

  std::unique_ptr<Type>& ptrHolder(hmTypes[strName]);
  ptrHolder.reset(Create(iKind, strName, pBaseType));
  return ptrHolder.get();

}
/*************************************************************************/
}
}
