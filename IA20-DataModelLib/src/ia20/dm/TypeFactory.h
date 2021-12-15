/*
 * File: TypeFactory.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_TypeFactory_H_
#define _IA20_DM_TypeFactory_H_

#include <ia20/commonlib/commonlib.h>

#include "Type.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The TypeFactory class.
 *
 */
class TypeFactory {
protected:
  static Type* Create(Type::Kind iKind, const String& strName, const Type* pParent);
};

/*************************************************************************/
}
}

#endif /* _IA20_DM_TypeFactory_H_ */
