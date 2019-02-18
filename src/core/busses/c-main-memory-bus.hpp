/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _C_MAIN_MEMORY_BUS_HPP_
#define _C_MAIN_MEMORY_BUS_HPP_

#include "../c-memory.hpp"

//
// The main 64K memory bus
//

class CMainMemoryBus: public CMemory 
{
public:
  	CMainMemoryBus(CMemory *ram, CMemory *iou, CMemory *ioBus, CMemory *lcBus);

    BYTE read(WORD addr);
    void write(WORD addr, BYTE byte);

protected:
	CMemory *ram;
	CMemory *iou;
	CMemory *ioBus;
	CMemory *lcBus;

	// Proxies
	CMemory *lcRom;	      // $D000-$FFFF rom
	CMemory *internalRom; // $C100-$CFFF internal rom

};

#endif // _C_MAIN_MEMORY_BUS_HPP_
