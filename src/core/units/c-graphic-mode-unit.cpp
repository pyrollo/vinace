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

#include "c-graphic-mode-unit.hpp"

//
// Graphic mode unit
//
// See "Apple IIe Technical Reference Manual" page 29 (PDF p63)

/*
C01A 49178 RDTEXT        ECG  R7  Status of Text/Graphics
C01B 49179 RDMIXED       ECG  R7  Status of Full Screen/Mixed Graphics
C01C 49180 RDPAGE2       ECG  R7  Status of Page 1/Page 2
C01D 49181 RDHIRES       ECG  R7  Status of LoRes/HiRes

C050 49232 TXTCLR       OECG WR   Display Graphics
C051 49233 TXTSET       OECG WR   Display Text
C052 49234 MIXCLR       OECG WR   Display Full Screen
C053 49235 MIXSET       OECG WR   Display Split Screen
C054 49236 TXTPAGE1     OECG WR   Display Page 1
C055 49237 TXTPAGE2     OECG WR   Display Page 2
C056 49238 LORES        OECG WR   Display LoRes Graphics
C057 49239 HIRES        OECG WR   Display HiRes Graphics
*/

void CGraphicModeUnit::reset() {
	s_text  = false;
	s_mixed = false;
	s_page2 = false;
	s_hires = false;
	notifyUpdate();
}

void CGraphicModeUnit::access(BYTE addr) {
	switch(addr) {
		CASE_SWITCH(text,  0x50, 0x51);
		CASE_SWITCH(mixed, 0x52, 0x53);
		CASE_SWITCH(page2, 0x54, 0x55);
		CASE_SWITCH(hires, 0x56, 0x57);
		default:return;
	}
	notifyUpdate();
}

void CGraphicModeUnit::write(BYTE addr, BYTE byte) {
	access(addr);
}

BYTE CGraphicModeUnit::read(BYTE addr) {
	access(addr);
	switch (addr) {
		CASE_READ(text,  0x1A);
		CASE_READ(mixed, 0x1B);
		CASE_READ(page2, 0x1C);
		CASE_READ(hires, 0x1D);
	}
	return dummy_byte();
}