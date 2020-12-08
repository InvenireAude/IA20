/*
 * File: PacketFactory.cpp
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */


#include "PacketFactory.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

thread_local PacketFactory *PacketFactory::ThePacketFactory;

}
}
}
}