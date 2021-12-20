/*
 * File: Exception.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Exception_H_
#define _IA20_IOT_Exception_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Exception class.
 *
 */
class Exception : public IA20::Exception {
public:

	virtual ~Exception() throw();
	Exception();
  Exception(const String& strInfo);

  virtual const char*  getName();

protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Exception_H_ */
