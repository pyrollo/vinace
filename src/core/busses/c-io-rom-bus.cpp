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

#include "c-io-rom-bus.hpp"

CIoRomBus::CIoRomBus(CIoRomUnit *ioRomUnit, CMemory *slotRom, CMemory *internalRom) {
	this->ioRomUnit   = ioRomUnit;
	this->slotRom     = slotRom;
	this->internalRom = internalRom;
	notify((CObservable *)ioRomUnit);
	ioRomUnit->subscribe((CObserver *)this);
}

CIoRomBus::~CIoRomBus() {
	ioRomUnit->unsubscribe((CObserver *)this);
}

BYTE CIoRomBus::read(WORD addr) {
	if ((addr&0xff00) == 0x0300)
		return selectedC3Rom->read(addr);
    else
		return selectedRom->read(addr);
}

void CIoRomBus::notify(CObservable * obs) {
	if (ioRomUnit->get_cxrom()) {
		selectedRom   = internalRom;
		selectedC3Rom = internalRom;
	} else {
		selectedRom = slotRom;
		if (ioRomUnit->get_c3rom()) {  
			selectedC3Rom = slotRom;
		} else {
			selectedC3Rom = internalRom;
		}
	}
}
