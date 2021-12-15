/*
 * File: StringType.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_StringType_H_
#define _IA20_DM_StringType_H_

#include <ia20/commonlib/commonlib.h>

#include "SimpleType.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The StringType class.
 *
 */
class DataObject;

class StringType : public SimpleType {
public:

	virtual ~StringType() throw();

	StringType(const String& strName, const Type* pBaseType = NULL);

  virtual DataObject* create(DataObject *pParent = NULL)const;

  static const String&    CDefaultName;
  static const Type::Kind CKind = Type::CStringType;

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_StringType_H_ */
