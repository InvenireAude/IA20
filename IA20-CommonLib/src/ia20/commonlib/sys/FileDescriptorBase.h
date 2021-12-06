/*
 * File: FileDescriptorBase.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_SYS_FileDescriptorBase_H_
#define _IA20_SYS_FileDescriptorBase_H_

namespace IA20 {
namespace SYS {

/*************************************************************************/
/** The FileDescriptorBase class.
 *
 */
class FileDescriptorBase {
public:

	virtual ~FileDescriptorBase() throw();

  // inline int& operator int(){
  //   return fd;
  // }

//protected:

  int iFileDescriptor;
};
/*************************************************************************/
}
}

#endif /* _IA20_SYS_FileDescriptorBase_H_ */
