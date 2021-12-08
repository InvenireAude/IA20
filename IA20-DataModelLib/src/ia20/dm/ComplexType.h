/*
 * File: ComplexType.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ComplexType_H_
#define _IA20_DM_ComplexType_H_

#include <ia20/commonlib/commonlib.h>

#include "Type.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The ComplexType class.
 *
 */
class ComplexType : public Type {
public:

	~ComplexType() throw();
	ComplexType(Kind iKind);

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ComplexType_H_ */
