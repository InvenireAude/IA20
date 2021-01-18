/*
 * File: URingException.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_URing_URingException_H_
#define _IA20_URing_URingException_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace URing {

/*************************************************************************/
/** The URingException class.
 *
 */
class URingException : public SystemException {
public:

	virtual ~URingException() throw();

	URingException(const String& strInfo, int iErrno);

  virtual const char*  getName();

protected:

};
/*************************************************************************/
}
}

#endif /* _IA20_URing_URingException_H_ */
