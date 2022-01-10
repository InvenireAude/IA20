/*
 * File: InternalException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_InternalException_H_
#define _IA20_IOT_InternalException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The InternalException class.
 *
 */
class InternalException : public IOT::Exception{
public:

	virtual ~InternalException() throw();
	InternalException();
	InternalException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_InternalException_H_ */
