/*
 * File: TypedValue.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_TypedValue_H_
#define _IA20_DM_TypedValue_H_

#include <ia20/commonlib/commonlib.h>

#include "Type.h"

namespace IA20 {
namespace DM {

namespace Proxy {
  class Setter;
  class Converter;
};

/*************************************************************************/
/** The TypedValue class.
 *
 */

class DataObject;

struct TypedValue {
public:

 ~TypedValue() throw();
	TypedValue();

  union Holder {
    Type::Integer iValue;
    Type::Long    lValue;
    Type::Boolean bValue;
    Type::String  sValue;
    DataObject*   pDataObject;

    inline Holder(Type::Integer iValue):iValue(iValue){};
    inline Holder(Type::String  sValue):sValue(sValue){};
    inline Holder(DataObject*   pDataObject = NULL):pDataObject(pDataObject){};
  };

  const Type* pType;
  Holder      mHolder;

};
/*************************************************************************/
}
}

#endif /* _IA20_DM_TypedValue_H_ */
