/*
 * File: LogLevel.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "LogLevel.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

const char* LogLevel::CLevelEnv_IOTEXPRESS("IA20_DBG_IOTEXPRESS");

LogLevel LogLevel::INSTANCE=LogLevel();

/*************************************************************************/
}
}
