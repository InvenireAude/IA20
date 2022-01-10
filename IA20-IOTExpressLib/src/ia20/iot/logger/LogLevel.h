/*
 * File: LogLevel.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_LogLevel_H_
#define _IA20_IOT_LogLevel_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/commonlib/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The LogLevel class.
 *
 */
class LogLevel : public LogLevelBase {

  public:

	  static LogLevel INSTANCE; // The library scope instance
    static const char* CLevelEnv_IOTEXPRESS;

  LogLevel():
    LogLevelBase(CLevelEnv_IOTEXPRESS){};

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_LogLevel_H_ */
