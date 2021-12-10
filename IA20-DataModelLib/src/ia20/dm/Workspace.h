/*
 * File: Workspace.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Workspace_H_
#define _IA20_DM_Workspace_H_

#include <ia20/commonlib/commonlib.h>


#include "DataObjectContainer.h"
#include "DataObject.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The Workspace class.
 *
 */
class Workspace : public DataObject {
public:

	virtual ~Workspace() throw();
	Workspace();

  inline void add(DataObject* pDataObject){
    mContainer.append(pDataObject);
  }

protected:
  DataObjectContainer mContainer;

};
/*************************************************************************/
}
}

#endif /* _IA20_DM_Workspace_H_ */
