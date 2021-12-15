/*
 * File: ItemNotFoundException.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_ItemNotFoundException_H_
#define _IA20_DM_ItemNotFoundException_H_

#include <ia20/commonlib/commonlib.h>

#include "Exception.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The ItemNotFoundException class.
 *
 */
class ItemNotFoundException : public DM::Exception{
public:

	virtual ~ItemNotFoundException() throw();
	ItemNotFoundException();
	ItemNotFoundException(const String& strInfo);

  virtual const char*  getName();
protected:

};

/*************************************************************************/
}
}

#endif /* _IA20_DM_ItemNotFoundException_H_ */
