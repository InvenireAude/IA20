/*
 * File: DataObjectContainer.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_DataObjectContainer_H_
#define _IA20_DM_DataObjectContainer_H_

#include <ia20/commonlib/commonlib.h>

#include "TypedValue.h"

namespace IA20 {
namespace DM {

/*************************************************************************/
/** The DataObjectContainer class.
 *
 */
class DataObjectContainer {
public:

  ~DataObjectContainer();
	DataObjectContainer();

  typedef uint32_t SizeType;

  inline SizeType getSize()const{
    return iSize;
  }

  inline void set(unsigned int iIdx, DataObject* pDataObject){

    if(iSize <= iIdx)
      iSize = iIdx + 1;

    tabDataObjects[iIdx] = pDataObject;
  }

  inline int append(DataObject* pDataObject){

    tabDataObjects[iSize] = pDataObject;

    return ++iSize;
  }

  inline DataObject* get(unsigned int iIdx)const{
    return tabDataObjects[iIdx];
  }

protected:
  SizeType iSize;
  DataObject **tabDataObjects;
};

/*************************************************************************/
}
}

#endif /* _IA20_DM_DataObjectContainer_H_ */
