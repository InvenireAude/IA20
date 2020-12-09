/*
 * File: LogFileAllocator.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_LogFileAllocator_H_
#define _IA20_Net_Engine_Raft_LogFileAllocator_H_

#include <ia20/commonlib/commonlib.h>

#include "Definitions.h"

#include "LogFile.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogEntry;

/*************************************************************************/
/** The LogFileAllocator class.
 *
 */
class LogFileAllocator : public LogFile {
public:

	virtual ~LogFileAllocator() throw();

	LogFileAllocator(const String& strFileName, size_t iSequenceId, size_t iSize);

protected:

  String  strFileName;
  std::unique_ptr<SharedMemoryFile> ptrSHM;
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFileAllocator_H_ */
