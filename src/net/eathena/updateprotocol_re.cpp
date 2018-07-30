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

#include "net/eathena/updateprotocol_re.h"

#include "logger.h"

#include "net/eathena/network.h"
#include "net/eathena/protocolout.h"

#include "debug.h"

extern int packetVersion;
extern bool packets_re;

namespace EAthena
{

void updateProtocolRe()
{
#include "net/protocoloutupdate.h"
#include "net/eathena/packetsout_shuffle_re.inc"
#undef packet
}

PACKETSOUT_SHUFFLE_RE_VOID
PROTOCOLOUTUPDATE_VOID
PROTOCOLOUT_VOID

}  // namespace EAthena
