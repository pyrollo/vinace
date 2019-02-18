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

#include "c-language-card-unit.hpp"

//
// Language Card unit
//

/*
C011 49169 RDLCBNK2      ECG  R7  Status of Selected $Dx Bank
C012 49170 RDLCRAM       ECG  R7  Status of $Dx ROM / $Dx RAM

C080 49280              OECG  R   Read RAM bank 2; no write
C081 49281 ROMIN        OECG  RR  Read ROM; write RAM bank 2
C082 49282              OECG  R   Read ROM; no write
C083 49283 LCBANK2      OECG  RR  Read/write RAM bank 2
C084 49284              OECG  R   Read RAM bank 2; no write
C085 49285 ROMIN        OECG  RR  Read ROM; write RAM bank 2
C086 49286              OECG  R   Read ROM; no write
C087 49287 LCBANK2      OECG  RR  Read/write RAM bank 2
C088 49288              OECG  R   Read RAM bank 1; no write
C089 49289              OECG  RR  Read ROM; write RAM bank 1
C08A 49290              OECG  R   Read ROM; no write
C08B 49291              OECG  RR  Read/write RAM bank 1
C08C 49292              OECG  R   Read RAM bank 1; no write
C08D 49293              OECG  RR  Read ROM; write RAM bank 1
C08E 49294              OECG  R   Read ROM; no write
C08F 49295              OECG  RR  Read/write RAM bank 1
*/

void CLanguageCardUnit::reset() {
	s_lcbnk2  = false;
	s_lcram   = false;
	s_lcwrite = false;
	s_lcwchg  = false;
	notifyUpdate();
}

void CLanguageCardUnit::access(BYTE addr) {
	// Addresses from 0x80 to 0x8f
	if ((addr&0xf0) == 0x80) {
		s_lcbnk2  = addr&0x08; // Ram bank
		switch(addr&0x03) {
			case 0:
				s_lcram   = true;
				s_lcwrite = false;
				s_lcwchg  = false;
				break;
			case 1:
				s_lcram   = false;
				s_lcwrite = true;
				s_lcwchg  = true;
				break;
			case 2:
				s_lcram   = false;
				s_lcwrite = false;
				s_lcwchg  = false;
				break;
			case 3:
				s_lcram   = s_lcwchg or s_lcram;
				s_lcwrite = s_lcwchg or s_lcwrite;
				s_lcwchg  = true; // Will be ok for the next time
				break;
		}
		notifyUpdate();
	}
}

void CLanguageCardUnit::write(BYTE addr, BYTE byte) {
	access(addr);
}

BYTE CLanguageCardUnit::read(BYTE addr) {
	access(addr);
	switch (addr) {
		CASE_READ(lcbnk2, 0x11);
		CASE_READ(lcram,  0x12);
	}
	return dummy_byte();
}
