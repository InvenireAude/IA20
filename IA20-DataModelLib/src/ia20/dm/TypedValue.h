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

/*************************************************************************/
/** The TypedValue class.
 *
 */

class DataObject;

class TypedValue {
public:

 ~TypedValue() throw();
	TypedValue();

  union Holder {
    Type::Integer iValue;
    Type::Long    lValue;
    Type::Boolean bValue;

    DataObject*   pDataObject;
  };

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_TypedValue_H_ */
