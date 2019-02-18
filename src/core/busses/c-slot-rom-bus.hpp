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

#ifndef _C_SLOT_ROM_BUS_HPP_
#define _C_SLOT_ROM_BUS_HPP_

#include "../c-memory.hpp"

class CSlotRomBus: public CMemory
{
public:
	CSlotRomBus();

	BYTE read(WORD addr);

	void insert_card(int slot, CMemory *cardRom, CMemory *cardRomExt);
	void remove_card(int slot);

private:
    // Pointer toward cards roms
	CMemory *rom[8];
	CMemory *romExt[8];

	int selectedRom;
	
	CMemory *dummy;
};

#endif // _C_SLOT_ROM_MEMORY_BUS_HPP_
