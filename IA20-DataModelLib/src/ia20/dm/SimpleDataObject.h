/*
 * File: SimpleDataObject.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_SimpleDataObject_H_
#define _IA20_DM_SimpleDataObject_H_

#include <ia20/commonlib/commonlib.h>

#include "DataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The SimpleDataObject class.
 *
 */
class SimpleDataObject : public DataObject {
public:

	virtual ~SimpleDataObject() throw();

protected:
	SimpleDataObject(const Type* pType, DataObject *pParent);

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_SimpleDataObject_H_ */
