/*
 * File: Logger.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Mocker_Logger_H_
#define _IA20_Net_Engine_Raft_Mocker_Logger_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/conn/mcast/Sender.h>
#include <ia20/net/conn/mcast/Receiver.h>

#include "../LogFileWriter.h"

#include <list>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
/** The Logger class.
 *
 */
class Logger :
  public LogFileWriter {
public:

	virtual ~Logger() throw();

 	Logger(size_t iMemorySize = 1000000);

  void simpleDump(std::ostream& os);

protected:

};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Mocker_Logger_H_ */
