/*
 * File: TypeFactory.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "TypeFactory.h"

#include <ia20/dm/exception/InvalidTypeDefinitionException.h>

#include "IntegerType.h"
#include "StringType.h"
#include "ComplexType.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
Type* TypeFactory::Create(Type::Kind iKind, const String& strName, const Type* pParent){

  switch(iKind){

    case Type::CIntegerType:
      return new IntegerType(strName, pParent);

    case Type::CStringType:
      return new StringType(strName, pParent);

    case Type::CDataObjectType:
      return new ComplexType(strName, pParent);

    default:
      IA20_THROW(InvalidTypeDefinitionException("Unsupported type kind: ")<<iKind);
  }

}
/*************************************************************************/
}
}
