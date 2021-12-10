/*
 * File: StringDataObject.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_StringDataObject_H_
#define _IA20_DM_StringDataObject_H_

#include <ia20/commonlib/commonlib.h>

#include "SimpleDataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The StringDataObject class.
 *
 */
class StringDataObject : public SimpleDataObject{
public:

	virtual ~StringDataObject() throw();

	StringDataObject(const Type* pType, DataObject *pParent = NULL);



  virtual Type::Integer getInteger()const;
  virtual Type::CString getCString()const;

  virtual void setString(const String& strValue);

  virtual void          saveToStream(std::ostream& os)const;

protected:

};
/*************************************************************************/
}
}

#endif /* _IA20_DM_StringDataObject_H_ */
