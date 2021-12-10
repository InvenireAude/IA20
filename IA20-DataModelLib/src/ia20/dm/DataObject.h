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

  inline const Type* getType()const{
    return mValue.pType;
  }

  inline bool isRoot()const{
    return pParent == NULL;
  }

  DataObject* getParent()const;

  virtual Type::Integer getInteger()const;
  virtual Type::CString getCString()const;

  virtual void setInteger(Type::Integer iValue);
  virtual void setString(const String& strValue);

  virtual void saveToStream(std::ostream& os)const;

  virtual void createProperty(unsigned int iIdx);

  virtual void setProperty(unsigned int iIdx, DataObject* pDataObject);
  virtual void setProperty(const String& strName, DataObject* pDataObject);

  virtual DataObject* getProperty(unsigned int iIdx)const;
  virtual DataObject* getProperty(const String& strName)const;

protected:

	DataObject(const Type* pType, DataObject *pParent = NULL);

  DataObject *pParent;
  TypedValue mValue;

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataObject_H_ */
