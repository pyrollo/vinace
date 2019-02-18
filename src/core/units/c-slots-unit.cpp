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

#include "c-slots-unit.hpp"

CSlotsUnit::CSlotsUnit() {
	for (int index = 0; index <7; index++)
		slots[index] = 0;
}

// Spread reset signal to cards
void CSlotsUnit::reset() {
	for (int index = 0; index <7; index++)
		if (slots[index]) slots[index]->reset();
	notifyUpdate();
}

// Dispatch write requests to cards
void CSlotsUnit::write(BYTE addr, BYTE byte) {
	if (addr > 0x8F) {
		if (slots[(addr>>4)-0x9]) {
			slots[(addr>>4)-0x9]->write(addr&0x0f, byte);
		}
	}
}

// Dispatch read requests to cards
BYTE CSlotsUnit::read(BYTE addr) {
	if (addr > 0x8F) {
		if (slots[(addr>>4)-0x9]) {
			return slots[(addr>>4)-0x9]->read(addr&0x0f);
		} 
	}
	return dummy_byte()&0x7F;
}

void CSlotsUnit::insert_card(int slot, CUnit *cardUnit) {
	if (slot > 0 and slot < 8)
		slots[slot-1] = cardUnit;
	notifyUpdate();
}

void CSlotsUnit::remove_card(int slot) {
	if (slot > 0 and slot < 8)
		slots[slot-1] = 0;
	notifyUpdate();
}
/*
CCard *CSlotsUnit::get_card(int slot) {
	if (slot >= 0 and slot < 8)
		return slots[slot];
}
*/