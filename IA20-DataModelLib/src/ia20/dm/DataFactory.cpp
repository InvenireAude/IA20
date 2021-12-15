/*
 * File: DataFactory.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "DataFactory.h"

#include <ia20/dm/exception/InvalidTypeDefinitionException.h>
#include <ia20/dm/exception/ItemNotFoundException.h>

#include "Namespace.h"
#include "Type.h"

#include "IntegerType.h"
#include "StringType.h"
#include "ComplexType.h"
#include "Namespace.h"

namespace IA20 {
namespace DM {


DataFactory* DataFactory::TheDefaultInstance;
/*************************************************************************/
DataFactory::DataFactory(const DataFactory* pParent):
  hmNamespaces(20),
  hmQuickLookUp(200),
  pParent(pParent){
	IA20_TRACER;
}
/*************************************************************************/
DataFactory::~DataFactory() throw(){
	IA20_TRACER;
}
/*************************************************************************/
Namespace* DataFactory::createNamespace(const String& strNamespace){
	IA20_TRACER;

  NamespaceMap::iterator it = hmNamespaces.find(strNamespace);

  if(it != hmNamespaces.end()){
    IA20_THROW(InvalidTypeDefinitionException("Namespace already exists: [")<<strNamespace<<"]");
  }

  std::unique_ptr<Namespace>& ptrHolder(hmNamespaces[strNamespace]);
  ptrHolder.reset(new Namespace(strNamespace));
  return ptrHolder.get();
}

/*************************************************************************/
Type* DataFactory::createType(const String& strNamespace, Type::Kind iKind, const String& strType, const Type* pBaseType){
  IA20_TRACER;

  Namespace* pNamespace = getNamespace(strNamespace);
  std::unique_ptr<Type> ptrType(pNamespace->createType(iKind, strType, pBaseType));

  Tools::Hash::KeyPairType key(pNamespace->getName(), ptrType->getName());

  hmQuickLookUp[key] = ptrType.get();

  return ptrType.release();
}
/*************************************************************************/
Namespace* DataFactory::getNamespace(const String& strNamespace){
  IA20_TRACER;

  NamespaceMap::iterator it = hmNamespaces.find(strNamespace);

  if(it == hmNamespaces.end()){
    IA20_THROW(ItemNotFoundException("Namespace does not exists: [")<<strNamespace<<"]");
  }

  return it->second.get();
}
/*************************************************************************/
const Type* DataFactory::getType(const String& strNamespace, const String& strType)const{
  IA20_TRACER;

  IA20_LOG(true, "Get type:["<<strNamespace<<"#"<<strType<<"]"<<hmQuickLookUp.size()<<", addr: "<<(void*)(this));

  Tools::Hash::KeyPairType key(strNamespace, strType);

  QuickLookUpMap::const_iterator it = hmQuickLookUp.find(key);

  if(it == hmQuickLookUp.end()){
    if(pParent){
      return pParent->getType(strNamespace, strType);
    }else{
      IA20_THROW(ItemNotFoundException("(namespace, type) does not exists: [")<<strNamespace<<","<<strType<<"]");
    }
  }

  return it->second;
}
/*************************************************************************/
Type* DataFactory::getTypeImpl(const String& strNamespace, const String& strType){
  IA20_TRACER;

  IA20_LOG(true, "Get type impl:["<<strNamespace<<"#"<<strType<<"]"<<hmQuickLookUp.size()<<", addr: "<<(void*)(this));

  Tools::Hash::KeyPairType key(strNamespace, strType);

  QuickLookUpMap::iterator it = hmQuickLookUp.find(key);

  if(it == hmQuickLookUp.end()){
      IA20_THROW(ItemNotFoundException("(namespace, type) does not exists: [")<<strNamespace<<","<<strType<<"]");
  }

  return it->second;
}
/*************************************************************************/
void DataFactory::setupDefaultTypes(){
  IA20_TRACER;

  createNamespace(Namespace::CDefaultName);
  createType(Namespace::CDefaultName, IntegerType::CKind, IntegerType::CDefaultName);
  createType(Namespace::CDefaultName, StringType::CKind, StringType::CDefaultName);

}
/*************************************************************************/
}
}
