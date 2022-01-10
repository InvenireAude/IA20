/*
 * File: Message.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Message.h"

#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {

const size_t Message::CAlignedSize(
	sizeof(std::aligned_storage_t< sizeof(Message), alignof(Message) >));

/*************************************************************************/
}
}
