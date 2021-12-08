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
class Type {
public:

  typedef int32_t Integer;
  typedef int64_t Long;
  typedef double  Float;
  typedef bool    Boolean;
  typedef const char* String;

  enum Kind {
    CNoneType          = 0x00,
    CIntegerType       = 0x01,
    CLongType          = 0x03,
    CFloatType         = 0x04,
    CStringType        = 0x05,
    CDataObjectType    = 0x06,
    CNumberOfTypeKinds = 0x06,
  };


	~Type() throw();

  inline Kind getKind()const{
    return iKind;
  }

protected:
	Type(Kind iKind);


  Kind iKind;

};

/*************************************************************************/

#endif /* _Type_H_ */

