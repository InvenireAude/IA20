/*
 * File: DataObjectArray.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_DataObjectArray_H_
#define _IA20_DM_DataObjectArray_H_

#include <ia20/commonlib/commonlib.h>

#include "DataObject.h"
#include "DataObjectContainer.h"

namespace IA20 {
namespace DM {


/*************************************************************************/
/** The DataObjectArray class.
 *
 */
class DataObjectArray : public DataObject {
public:

	virtual ~DataObjectArray() throw();
	DataObjectArray(const Type* pType, DataObject *pParent = NULL);

protected:

  DataObjectContainer mContainer;
};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataObjectArray_H_ */
