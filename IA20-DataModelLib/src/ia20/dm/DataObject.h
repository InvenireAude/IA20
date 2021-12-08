/*
 * File: DataObject.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_DataObject_H_
#define _IA20_DM_DataObject_H_

#include <ia20/commonlib/commonlib.h>

#include "TypedValue.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The DataObject class.
 *
 */
class DataObject {
public:

  ~DataObject();

	DataObject(DataObject *pParent, const TypedValue& mValue);

protected:

  DataObject *pParent;
  TypedValue mValue;

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataObject_H_ */
