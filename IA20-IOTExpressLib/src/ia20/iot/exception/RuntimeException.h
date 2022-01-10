/*
 * File: RuntimeException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_RuntimeException_H_
#define _IA20_IOT_RuntimeException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The RuntimeException class.
 *
 */
class RuntimeException : public IOT::Exception{
public:

	virtual ~RuntimeException() throw();
	RuntimeException();
	RuntimeException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_RuntimeException_H_ */
