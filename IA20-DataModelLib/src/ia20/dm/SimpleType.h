/*
 * File: SimpleType.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_SimpleType_H_
#define _IA20_DM_SimpleType_H_

#include <ia20/commonlib/commonlib.h>

#include "Type.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The SimpleType class.
 *
 */
class SimpleType : public Type{
public:

	virtual ~SimpleType() throw();


	SimpleType(Kind iKind, const String& strName);
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_SimpleType_H_ */
