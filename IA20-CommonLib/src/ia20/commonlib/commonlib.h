/*
 * File: IA20-CommonLib/src/commonlib.h
 *
 * Copyright (C) 2015, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef __IA20_COMMONLIB_H_
#define __IA20_COMMONLIB_H_

#include "types.h"
#include "config.h"

#include "logger/logger.h"

#include "memory/memory.h"

#include "memory/ma/Allocator.h"

#include "memory/mm/MemoryManager.h"
#include "memory/mm/FixedObjectPoolMemoryManager.h"
#include "memory/mm/ContinuousMemoryMananger.h"

//#include "containers/HashMapStringPairToPointer.h"

#include "memory/sharable/SharedContent.h"

#include "exception/Exception.h"
#include "exception/DataException.h"
#include "exception/EndOfDataException.h"
#include "exception/InternalException.h"
#include "exception/MemoryException.h"
#include "exception/SystemException.h"
#include "exception/ConfigException.h"
#include "exception/ItemNotFoundException.h"
#include "exception/AlreadyExists.h"
#include "exception/BadUsageException.h"

#include "threads/Thread.h"
#include "threads/Runnable.h"
#include "threads/Mutex.h"
#include "threads/Condition.h"

#include "time/Timestamp.h"
#include "time/Time.h"
#include "time/DateTime.h"
#include "time/Date.h"

#include "tools/Buffer.h"
#include "tools/MiscTools.h"
#include "tools/TypeTools.h"
#include "tools/EnvTools.h"

#include "time/TimeSamplesRegister.h"

#include "types/BitMap.h"
#include "types/Decimal.h"
#include "types/DoubleLinkedList.h"
#include "types/SortedHeap.h"
#include "types/ProducerConsumerQueue.h"
#include "types/BufferQueue.h"

#include "sys/Signal.h"

#include "callbacks/CallbacksRegister.h"
#include "testcase/TestUnit.h"
#include "testcase/TestSuite.h"

#include "memory/InstanceFeature.h"

#include "ui/UserMessage.h"
#include "ui/MessageCatalog.h"

#include "version/Version.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif /*COMMONLIB_H_*/
