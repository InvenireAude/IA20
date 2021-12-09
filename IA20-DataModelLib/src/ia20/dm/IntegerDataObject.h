/*
 * File: IntegerDataObject.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_IntegerDataObject_H_
#define _IA20_DM_IntegerDataObject_H_

#include <ia20/commonlib/commonlib.h>

#include "SimpleDataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The IntegerDataObject class.
 *
 */
class IntegerDataObject : public SimpleDataObject{
public:

	virtual ~IntegerDataObject() throw();
	IntegerDataObject(const Type* pType, DataObject *pParent  = NULL);

  virtual Type::Integer getInteger()const;
  virtual void          saveToStream(std::ostream& os)const;

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_IntegerDataObject_H_ */
