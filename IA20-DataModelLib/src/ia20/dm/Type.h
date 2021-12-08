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
  // typedef const char* String;

  enum Kind {
    CNone       = 0x00,
    CInteger    = 0x01,
    CLong       = 0x03,
    CFloat      = 0x04,
    CString     = 0x05,
    CDataObject = 0x10,
  };


	~Type() throw();
	Type(Kind iKind);

protected:

  Kind iKind;

};

/*************************************************************************/

#endif /* _Type_H_ */
