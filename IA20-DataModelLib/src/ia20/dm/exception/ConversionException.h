/*
 * File: ConversionException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ConversionException_H_
#define _IA20_DM_ConversionException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The ConversionException class.
 *
 */
class ConversionException : public DM::Exception{
public:

	virtual ~ConversionException() throw();
	ConversionException();
	ConversionException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ConversionException_H_ */
