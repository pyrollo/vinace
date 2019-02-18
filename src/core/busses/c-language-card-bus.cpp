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

#include "c-language-card-bus.hpp"
#
CLanguageCardBus::CLanguageCardBus(CLanguageCardUnit *unit, CMemory *rom12k, CMemory *ram16k) {
	this->unit = unit;
	
	this->loRom  = rom12k;
	this->hiRom  = new CMemoryProxy(rom12k, 0x1000);
	this->loRam1 = ram16k;
	this->loRam2 = new CMemoryProxy(ram16k, 0x1000);
	this->hiRam  = new CMemoryProxy(ram16k, 0x2000);

	unit->subscribe(this);
	
	// Suppose settings have changed
	notify((CObservable *)unit);
}

CLanguageCardBus::~CLanguageCardBus() {
	unit->unsubscribe(this);
	delete hiRom;
	delete loRam2;
	delete hiRam;
}

BYTE CLanguageCardBus::read(WORD addr) {
	if (addr <0x1000) return loMemRead->read(addr);
	if (addr <0x3000) return hiMemRead->read(addr-0x1000);
}

void CLanguageCardBus::write(WORD addr, BYTE byte) {
	if (addr <0x1000 and loMemWrite) {loMemWrite->write(addr, byte); return; }
	if (addr <0x3000 and hiMemWrite) {hiMemWrite->write(addr-0x1000, byte); return; }
}

// Language card settings have changed, so change memory mapping

void CLanguageCardBus::notify(CObservable * obs) {
	if (unit->get_lcram()) {
		loMemRead = (unit->get_lcbnk2())?loRam2:loRam1;
		hiMemRead = hiRam;
	} else {
		loMemRead = loRom;
		hiMemRead = hiRom;
	}

	if (unit->get_lcwrite()) {
		loMemWrite = (unit->get_lcbnk2())?loRam2:loRam1;
		hiMemWrite = hiRam;
	} else {
		loMemWrite = 0;
		hiMemWrite = 0;
	}
}

