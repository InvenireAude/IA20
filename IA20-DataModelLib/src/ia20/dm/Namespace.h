/*
 * File: Namespace.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Namespace_H_
#define _IA20_DM_Namespace_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/dm/tools/Hash.h>

#include <unordered_map>
#include <memory>

#include "TypeFactory.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The Namespace class.
 *
 */
class Namespace : public TypeFactory{
public:

	virtual ~Namespace() throw();

	Namespace(const String& strName);

  Type* createType(Type::Kind iKind, const String& strName, const Type* pBaseType);

  inline const String& getName()const{
    return strName;
  }

 static const String& CDefaultName;

protected:

  //TODO hashed string index

  typedef std::unordered_map<String, std::unique_ptr<Type>, Tools::Hash::StringHash > TypeMap;

  TypeMap hmTypes;
  String  strName;

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_Namespace_H_ */
