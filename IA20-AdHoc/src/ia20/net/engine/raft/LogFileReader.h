/*
 * File: LogFileReader.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_LogFileReader_H_
#define _IA20_Net_Engine_Raft_LogFileReader_H_

#include <ia20/commonlib/commonlib.h>

#include "Definitions.h"
#include "LogFile.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogEntry;

/*************************************************************************/
/** The LogFileReader class.
 *
 */
class LogFileReader :
  public LogFile {
public:

	virtual ~LogFileReader() throw();

	LogFileReader(const String& strFileName);

  void dump(std::ostream& os)const;

protected:


};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFileReader_H_ */
