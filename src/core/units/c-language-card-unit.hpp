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

#ifndef _C_LANGUAGE_CARD_UNIT_HPP_
#define _C_LANGUAGE_CARD_UNIT_HPP_

#include "c-unit.hpp"

//
// Language Card Unit
//
// This unit takes care of $D000-$FFFF memory management switches
//
// See "Apple IIe Technical Reference Manual" pages 79 to 83 (PDF pp113-117)

class CLanguageCardUnit: public CUnit {
public:
	void reset();
	BYTE read(BYTE addr);
	void write(BYTE addr, BYTE byte);

	// Language Card ($D000-DFFF and $E000-FFFF spaces)
	FLAG_RO(lcbnk2);   // Using $D000-$DFFF RAM bank 1 (0) or bank 2 (1)
	FLAG_RO(lcram);	   // Reading ROM(0) or RAM (1)
	FLAG_RO(lcwrite);  // Allow writing into RAM (1) or not (0)
	FLAG_RO(lcwchg);   // lcwrite changeable (1) or not (0) (to switch write, it is needed to switch twice)

private:
    void access(BYTE addr);
};

#endif // _C_LANGUAGE_CARD_UNIT_HPP_
