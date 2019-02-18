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

#include "c-keyboard-unit.hpp"

//
// Keyboard unit
//

/*
C000 49152 KBD          OECG  R   Last Key Pressed + 128
C010 49168 KBDSTRB      OECG WR   Keyboard Strobe
*/

void CKeyboardUnit::reset() {
	// TODO : check real behavior if a key is held down during reset
	lastkey = 0x00;
	keydown = false;
	keywaiting = false;	
	notifyUpdate();
}

void CKeyboardUnit::access(BYTE addr) {
	if (addr == 0x10) {
		keywaiting = false;
		notifyUpdate();
	}
}

void CKeyboardUnit::write(BYTE addr, BYTE byte) {
	access(addr);
}

BYTE CKeyboardUnit::read(BYTE addr) {
	access(addr);
	if (addr == 0x00)
		return bool_to_b7(lastkey, keywaiting);
	if (addr == 0x10)
		return bool_to_b7(lastkey, keydown);
	return dummy_byte(); 
}

void CKeyboardUnit::press_key(BYTE key) {
	lastkey = key;
	keydown = true;
	keywaiting = true;
	notifyUpdate();
}

void CKeyboardUnit::release_key() {
	keydown = false;
	notifyUpdate();
}

bool CKeyboardUnit::key_waiting() {
	return keywaiting;
}

