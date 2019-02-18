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

#ifndef _C_KEYBOARD_UNIT_HPP_
#define _C_KEYBOARD_UNIT_HPP_

#include "c-unit.hpp"

//
// Keyboard
//

class CKeyboardUnit: public CUnit {
public:
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);
	
	void press_key(BYTE key);
	void release_key();
	bool key_waiting();

protected:
	BYTE lastkey;
	bool keydown;
	bool keywaiting;

private:
    void access(BYTE addr);
};


#endif // _C_KEYBOARD_UNIT_HPP_
