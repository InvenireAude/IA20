/*
 * File: ConfigException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ConfigException_H_
#define _IA20_IOT_ConfigException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The ConfigException class.
 *
 */
class ConfigException : public IOT::Exception{
public:

	virtual ~ConfigException() throw();
	ConfigException();
	ConfigException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ConfigException_H_ */
