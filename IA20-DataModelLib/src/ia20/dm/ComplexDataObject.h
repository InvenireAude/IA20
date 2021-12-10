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
class ComplexType;
/*************************************************************************/
/** The ComplexDataObject class.
 *
 */
class ComplexDataObject : public DataObject {
public:

	virtual ~ComplexDataObject() throw();


	ComplexDataObject(const ComplexType* pType, DataObject *pParent = NULL);

  virtual void createProperty(unsigned int iIdx);
  virtual void setProperty(unsigned int iIdx, DataObject* pDataObject);
  virtual void setProperty(const String& strName, DataObject* pDataObject);

  virtual DataObject* getProperty(unsigned int iIdx)const;
  virtual DataObject* getProperty(const String& strName)const;

protected:
   const ComplexType* pComplexType;
  DataObject** tabDataObjects;
};
/*************************************************************************/
}
}

#endif /* _IA20_DM_ComplexDataObject_H_ */
