/*
 * File: IntegerType.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_IntegerType_H_
#define _IA20_DM_IntegerType_H_

#include <ia20/commonlib/commonlib.h>

#include "SimpleType.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The IntegerType class.
 *
 */
class IntegerType : public SimpleType{
public:

 ~IntegerType() throw();
	IntegerType();

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_IntegerType_H_ */
