/*
 * File: Logger.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Logger_H_
#define _IA20_Net_Engine_Raft_Logger_H_

#include <ia20/commonlib/commonlib.h>
#include "Definitions.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogFile;
class LogEntry;

/*************************************************************************/
/** The Logger class.
 *
 */
class Logger {
public:

	virtual ~Logger() throw();

  struct Configuration {
    String strPath;
    size_t iLogFileSize;

    Configuration(const Configuration& other):
      strPath(other.strPath),
      iLogFileSize(other.iLogFileSize){};


    Configuration& operator=(const Configuration& other){
      strPath = other.strPath;
      iLogFileSize = other.iLogFileSize;
      return *this;
    }

  };

  void appendEntry(TermType  iTerm,
                  IndexType iIndex,
                  LogEntrySizeType  iEntryDataSize,
                  const void* pSrcData);


	Logger(const Configuration& configuration);

protected:

  Configuration configuration;

  std::unique_ptr<LogFile> ptrActiveFile;

  size_t iFileIdx;

  void allocateFileIfNeeded(LogEntrySizeType iNewEntryDataSize);
};

/*****************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Logger_H_ */
