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

#include "c-main-memory-bus.hpp"

CMainMemoryBus::CMainMemoryBus(CMemory *ram, CMemory *iou, CMemory *ioBus, CMemory *lcBus) {
	this->ram = ram;
	this->iou = iou;
	this->ioBus = ioBus;
	this->lcBus = lcBus;
}

BYTE CMainMemoryBus::read(WORD addr) {

	// Language Card ($D000-$FFFF)
	if (addr >= 0xD000) return lcBus->read(addr-0xD000);

	// Main ram ($0000-$BFFF)
	if (addr < 0xC000) return ram->read(addr);

	// Slot rom
	if (addr >= 0xC100) return ioBus->read(addr-0xC000);
	
	// Input Output Unit
	return iou->read(addr-0xC000);
}

void CMainMemoryBus::write(WORD addr, BYTE byte) {

	// Main ram ($0000-$BFFF)
	if (addr < 0xC000) { ram->write(addr, byte); return; }

	// Language Card ($D000-$FFFF)
	if (addr >= 0xD000) { lcBus->write(addr-0xD000, byte); return; }

	// Input Output Unit
	if (addr <  0xC100) { iou->write(addr-0xC000, byte); return; }

	// Slot rom -> No write
};
