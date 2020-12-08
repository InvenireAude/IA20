/*
 * File: TimerWorker.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Unix_TimerWorker_H_
#define _IA20_Net_Engine_Raft_Unix_TimerWorker_H_

#include <ia20/commonlib/commonlib.h>

#include "../RaftEngine.h"
#include "../Message.h"

#include "Worker.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {


/*************************************************************************/
/** The TimerWorker class.
 *
 */
class TimerWorker : public Worker {
public:

	virtual ~TimerWorker() throw();

  virtual void run();

	TimerWorker(RaftEngine *pRaftEngine);

};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Unix_TimerWorker_H_ */
