/*
 * File: Type.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _Type_H_
#define _Type_H_

#include <ia20/commonlib/commonlib.h>


/*************************************************************************/
/** The Type class.
 *
 */

namespace IA20 {
namespace DM {

class ComplexType;
class DataObject;

class Type {
public:

  typedef int32_t Integer;
  typedef int64_t Long;
  typedef double  Float;
  typedef bool    Boolean;
  typedef const   char* CString;

  enum Kind {
    CNoneType          = 0x00,
    CIntegerType       = 0x01,
    CLongType          = 0x02,
    CFloatType         = 0x03,
    CStringType        = 0x04,
    CDataObjectType    = 0x05,
    CNumberOfTypeKinds = 0x06,
    CWorkspace         = 0xff
  };


	virtual ~Type() throw();

  inline Kind getKind()const{
    return iKind;
  }

  virtual const ComplexType* asComplexType()const;

  inline const String& getName()const{
    return strName;
  }

 virtual DataObject* create(DataObject *pParent = NULL) const = 0;

protected:
	Type(Kind iKind, const String& strName);
  Kind iKind;
  String strName;
};

/*************************************************************************/

}
}

#endif /* _Type_H_ */

