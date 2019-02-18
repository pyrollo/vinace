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

#include "c-game-unit.hpp"

//
// Game unit
//

/*
C040 49216 STROBE       OE    R   Game I/O Strobe Output
C058 49240 CLRAN0       OE G WR   Annunciator 0 Off
C059 49241 SETAN0       OE G WR   Annunciator 0 On
C05A 49242 CLRAN1       OE G WR   Annunciator 1 Off
C05B 49243 SETAN1       OE G WR   Annunciator 1 On
C05C 49244 CLRAN2       OE G WR   Annunciator 2 Off
C05D 49245 SETAN2       OE G WR   Annunciator 2 On
C05E 49246 CLRAN3       OE G WR   Annunciator 3 Off
C05F 49247 SETAN3       OE G WR   Annunciator 3 On
C061 49249 RDBTN0        ECG  R7  Switch Input 0 / Open Apple
C062 49250 BUTN1         E G  R7  Switch Input 1 / Solid Apple
C063 49251 RD63          E G  R7  Switch Input 2 / Shift Key
C064 49252 PADDL0       OECG  R7  Analog Input 0
C065 49253 PADDL1       OECG  R7  Analog Input 1
C066 49254 PADDL2       OE G  R7  Analog Input 2
C067 49255 PADDL3       OE G  R7  Analog Input 3
C070 49264 PTRIG         E    R   Analog Input Reset
*/

CGameUnit::CGameUnit() {
	// As there is not change during reset, it is important to init these flags
	s_pb0 = false;
	s_pb1 = false;
	s_pb2 = false;
}

void CGameUnit::reset() {
	// No reset for buttons or we loose their physical state
	s_an0 = false;
	s_an1 = false;
	s_an2 = false;
	s_an3 = false;
	notifyUpdate();
}

void CGameUnit::write(BYTE addr, BYTE byte) {
	access(addr);
}

BYTE CGameUnit::read(BYTE addr) {
	access(addr);
	switch(addr & 0x0f) {
		CASE_READ(pb0, 1);
		CASE_READ(pb1, 2);
		CASE_READ(pb2, 3);
		CASE_READ(paddl0, 4);
		CASE_READ(paddl1, 5);
		CASE_READ(paddl2, 6);
		CASE_READ(paddl3, 7);
	}
	return dummy_byte();
};

void CGameUnit::access(BYTE addr) {
	switch(addr) {
		case 0x40: break; // STROBE - Not managed
		case 0x70:
			s_paddl0 = true;
			s_paddl1 = true;
			s_paddl2 = true;
			s_paddl3 = true;
			s_ptrig = true;
			notifyUpdate();   // Give a chance to observers to see PTRIG change.
			s_ptrig = false;
			notifyUpdate();
			break; // PTRIG
		CASE_SWITCH(an0, 0x58, 0x59);
		CASE_SWITCH(an1, 0x5A, 0x5B);
		CASE_SWITCH(an2, 0x5C, 0x5D);
		CASE_SWITCH(an3, 0x5E, 0x5F);
		default:return; // Do not notify
	}
	notifyUpdate();
}

