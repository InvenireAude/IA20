/*
 * File: ConvertionException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ConvertionException_H_
#define _IA20_DM_ConvertionException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The ConvertionException class.
 *
 */
class ConvertionException : public DM::Exception{
public:

	virtual ~ConvertionException() throw();
	ConvertionException();
	ConvertionException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ConvertionException_H_ */
