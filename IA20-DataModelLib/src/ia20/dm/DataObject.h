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


  inline bool isRoot()const{
    return pParent == NULL;
  }

  DataObject* getParent()const;

  virtual Type::Integer getInteger()const;
  virtual Type::String  getString()const;
  virtual void          saveToStream(std::ostream& os)const;

  void setParent(DataObject *pParent);

  static DataObject* Create(void *pOwnerAddress, const Type* pType, DataObject *pParent = NULL);

protected:

	DataObject(const Type* pType, DataObject *pParent = NULL);

  DataObject *pParent;
  TypedValue mValue;

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataObject_H_ */
