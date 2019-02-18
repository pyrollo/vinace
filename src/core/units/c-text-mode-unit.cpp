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

#include "c-text-mode-unit.hpp"

//
// Text Mode Unit
//
// This unit takes care 80 column and charset switches
//
/*
C00C 49164 CLR80VID      ECG W    40 Columns
C00D 49165 SET80VID      ECG W    80 Columns
C00E 49166 CLRALTCHAR    ECG W    Primary Character Set
C00F 49167 SETALTCHAR    ECG W    Alternate Character Set
 
C01E 49182 RDALTCHAR     ECG  R7  Status of Primary/Alternate Character Set
C01F 49183 RD80VID       ECG  R7  Status of 40/80 Columns
*/

void CTextModeUnit::reset() {
	s_80vid = false;
	s_altchar = false;  
	notifyUpdate();
}

void CTextModeUnit::write(BYTE addr, BYTE byte) {
	switch(addr) {
		case 0x0C: s_80vid = false; break;
		case 0x0D: s_80vid = true; break;
		case 0x0E: s_altchar = false; break;
		case 0x0F: s_altchar = true; break;
		default:return;
	}
	notifyUpdate();
};

BYTE CTextModeUnit::read(BYTE addr) {
	switch (addr) {
		case 0x1E: return bool_to_b7(not s_altchar);
		case 0x1F: return bool_to_b7(s_80vid);
	}
	return dummy_byte();
};




