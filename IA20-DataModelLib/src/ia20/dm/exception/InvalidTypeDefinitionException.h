/*
 * File: InvalidTypeDefinitionException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_InvalidTypeDefinitionException_H_
#define _IA20_DM_InvalidTypeDefinitionException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The InvalidTypeDefinitionException class.
 *
 */
class InvalidTypeDefinitionException : public DM::Exception{
public:

	virtual ~InvalidTypeDefinitionException() throw();
	InvalidTypeDefinitionException();
	InvalidTypeDefinitionException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_InvalidTypeDefinitionException_H_ */
