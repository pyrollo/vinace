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

#include "c-slot-rom-bus.hpp"

CSlotRomBus::CSlotRomBus() {
	this->dummy = new CDummyMemory();
	
	// Actually, there is no slot #0 but it is used when no rom is selected
	for (int index = 0; index < 8; index ++) {
		rom[index]    = dummy; 
		romExt[index] = dummy;
	}
	
	selectedRom = 0;
}

void CSlotRomBus::insert_card(int slot, CMemory *cardRom, CMemory *cardRomExt) {
	if (slot > 0 and slot < 8) {
		rom[slot] = (cardRom)?cardRom:dummy;
		romExt[slot] = (cardRomExt)?cardRomExt:dummy;
	}
}

void CSlotRomBus::remove_card(int slot) {
	if (slot > 0 and slot < 8)
		rom[slot] = dummy;
		romExt[slot] = dummy;
}


// ROM management is explained (not so well) in IIe Reference Manual page 132
// Overview : when the card ROM is accessed, then its expansion ROM is visible
// in $C800-$CFFF.

BYTE CSlotRomBus::read(WORD addr) {
	// 256 bytes ROMs
	if (addr < 0x800) {
		selectedRom = (addr>>8);
		return rom[addr>>8]->read(addr&0xff);
	} else {
		if (addr == 0x0fff) // CLRROM - Rom extension selection cancel
			// TODO : check CLRROM behavior : which byte is returned by CLRROM?
			selectedRom = 0;
		return romExt[selectedRom]->read(addr-0x800);
	}
}
