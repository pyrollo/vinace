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

#include "c-io-rom-unit.hpp"

//
// Input Output Rom Unit
//
// This unit takes care of $C100-$CFFF memory management switches
//

/* 
C006 49158 SETSLOTCXROM  E G W    Peripheral ROM ($C100-$CFFF)
C007 49159 SETINTCXROM   E G W    Internal ROM ($C100-$CFFF)
C00A 49162 SETINTC3ROM   E G W    ROM in Slot 3
C00B 49163 SETSLOTC3ROM  E G W    ROM in Aux Slot

C015 49173 RDCXROM       E G  R7  Status of Periph/ROM Access
C017 49175 RDC3ROM       E G  R7  Status of Slot 3/Aux Slot ROM
*/

void CIoRomUnit::reset() {
	s_cxrom = false;
	s_c3rom = false;  
	notifyUpdate();
}

void CIoRomUnit::access(BYTE addr) {
	switch(addr) {
		CASE_SWITCH(cxrom, 0x06, 0X07);
//		CASE_SWITCH(c3rom, 0x0B, 0X0A);
		CASE_SWITCH(c3rom, 0x0A, 0X0B);
		default:return;
	}
	notifyUpdate();
}

void CIoRomUnit::write(BYTE addr, BYTE byte) {
	access(addr);
}

BYTE CIoRomUnit::read(BYTE addr) {
	access(addr);
	switch (addr) {
		CASE_READ(cxrom, 0x15);
		CASE_READ(c3rom, 0x17);
	}
	return dummy_byte();
}
