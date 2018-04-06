/*
 *  The ManaPlus Client
 *  Copyright (C) 2015-2018  The ManaPlus Developers
 *
 *  This file is part of The ManaPlus Client.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "net/eathena/updateprotocol.h"

#include "logger.h"

#include "net/eathena/network.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;
extern int packetVersionMain;
extern int packetVersionRe;
extern int packetVersionZero;
extern int serverVersion;
extern int evolPacketOffset;
extern bool packets_main;
extern bool packets_re;
extern bool packets_zero;

namespace EAthena
{

void updateProtocol()
{
    logger->log("packet version: %d", packetVersion);
    if (packets_main == true)
    {
        logger->log("packets version type: main");
        packetVersionMain = packetVersion;
        packetVersionRe = 0;
        packetVersionZero = 0;
    }
    else if (packets_re == true)
    {
        logger->log("packets version type: re");
        packetVersionRe = packetVersion;
        packetVersionMain = 0;
        packetVersionZero = 0;
    }
    else if (packets_zero == true)
    {
        logger->log("packets version type: zero");
        packetVersionZero = packetVersion;
        packetVersionMain = 0;
        packetVersionRe = 0;
    }
#define PACKETS_UPDATE
#include "net/protocoloutupdate.h"
#include "net/eathena/packetsout.inc"
#include "net/eathena/packetsout_shuffle_main.inc"
#include "net/eathena/packetsout_shuffle_zero.inc"
#undef packet
    Network *const network = Network::mInstance;
    if (network != nullptr)
    {
        network->clearHandlers();
        network->registerHandlers();
        network->registerFakeHandlers();
    }
}

PACKETSOUT_VOID
PACKETSOUT_SHUFFLE_MAIN_VOID
PACKETSOUT_SHUFFLE_ZERO_VOID
PROTOCOLOUTUPDATE_VOID
PROTOCOLOUT_VOID

}  // namespace EAthena
