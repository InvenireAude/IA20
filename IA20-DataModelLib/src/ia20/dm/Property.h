/*
 * File: Property.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */


#ifndef _IA20_DM_Property_H_
#define _IA20_DM_Property_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace DM {

class Type;

/*************************************************************************/
/** The Property class.
 *
 */
class Property {
public:

	virtual ~Property() throw();

	Property(const Type* pType, const String& strName,unsigned int iIdx);

  inline const String& getName()const{
    return strName;
  }

  inline const Type* getType()const{
    return pType;
  }

  unsigned int getIdx()const{
    return iIdx;
  }

protected:

  const Type* pType;
  unsigned int    iIdx;
  String strName;

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_Property_H_ */
