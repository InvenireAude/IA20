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

class LogFileWriter;
class LogEntry;

/*************************************************************************/
/** The Logger class.
 *
 */
class Logger :
  DoubleLinkedListOwner<LogFileWriter> {
public:

	virtual ~Logger() throw();

  struct Configuration {
    String strPath;
    size_t iLogFileSize;

    Configuration(String strPath, size_t iLogFileSize):
      strPath(strPath),
      iLogFileSize(iLogFileSize){};

    Configuration(const Configuration& other):
      strPath(other.strPath),
      iLogFileSize(other.iLogFileSize){};

    Configuration& operator=(const Configuration& other){
      strPath = other.strPath;
      iLogFileSize = other.iLogFileSize;
      return *this;
    }

  };

  const LogEntry* appendEntry(TermType  iTerm,
                        IndexType iIndex,
                        LogEntrySizeType  iEntryDataSize,
                        const void* pSrcData);

  void commit(const LogEntry* pLogEntry);

	Logger(const Configuration& configuration, ServerIdType iMyServerId);

  static void AllocateFile(const String& strFileName, size_t iSequenceId, size_t iSize);

protected:

  Configuration configuration;
  ServerIdType  iMyServerId;

  const LogFileWriter* getActiveLogFile()const;
  LogFileWriter* getActiveLogFile();

  void nextFileIfNeeded(LogEntrySizeType iNewEntryDataSize);
};

/*****************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Logger_H_ */
