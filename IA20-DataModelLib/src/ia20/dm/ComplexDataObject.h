/*
 * File: ComplexDataObject.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ComplexDataObject_H_
#define _IA20_DM_ComplexDataObject_H_

#include <ia20/commonlib/commonlib.h>

#include "DataObject.h"
namespace IA20 {
namespace DM {

/*************************************************************************/
/** The ComplexDataObject class.
 *
 */
class ComplexDataObject : public DataObject {
public:

	virtual ~ComplexDataObject() throw();


	ComplexDataObject(const Type* pType, DataObject *pParent = NULL);
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ComplexDataObject_H_ */
