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
class StringType : public SimpleType {
public:

	virtual ~StringType() throw();


	StringType();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_StringType_H_ */
