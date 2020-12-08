/*
 * File: LogEntry.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "LogEntry.h"


namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const LogEntry& entry){
  os<<" ["<<entry.iTerm;
  os<<","<<entry.iIndex;
  os<<"], sz: "<<entry.iEntryDataSize;
  os<<", at: "<<(void*)&entry;

  return os;
}
/*************************************************************************/
}
}
}
}
