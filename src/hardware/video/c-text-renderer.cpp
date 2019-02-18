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


#include "c-text-renderer.hpp"
// Needed for text flashing
#include <sys/time.h>
#include <iostream>

void CTextRenderer::set_charset(CCharset* charset) {
	this->charset = charset;
}

void  CTextRenderer::renderChar(CVideoOutput *vo, BYTE byte, int basex, int basey, bool flashon, bool dblwidth) {
	int x, y;
	BYTE *chr;
	BYTE mask;
	char_mode mode;
	
	chr = charset->get_char(byte);
	mode = charset->get_mode(byte);
	if (mode == mode_inverse or (mode == mode_flash and flashon))
		mask = 0x7f;
	else
		mask = 0x00;

	for (y=0; y<8; y++) {
		byte = chr[y]^mask;
		if (dblwidth) {
			vo->set_dpixel(basex+12, basey+y, (byte&0x01)?col_white:col_black);
			vo->set_dpixel(basex+10, basey+y, (byte&0x02)?col_white:col_black);
			vo->set_dpixel(basex+ 8, basey+y, (byte&0x04)?col_white:col_black);
			vo->set_dpixel(basex+ 6, basey+y, (byte&0x08)?col_white:col_black);
			vo->set_dpixel(basex+ 4, basey+y, (byte&0x10)?col_white:col_black);
			vo->set_dpixel(basex+ 2, basey+y, (byte&0x20)?col_white:col_black);
			vo->set_dpixel(basex   , basey+y, (byte&0x40)?col_white:col_black);
		} else {
			vo->set_pixel(basex+6, basey+y, (byte&0x01)?col_white:col_black);
			vo->set_pixel(basex+5, basey+y, (byte&0x02)?col_white:col_black);
			vo->set_pixel(basex+4, basey+y, (byte&0x04)?col_white:col_black);
			vo->set_pixel(basex+3, basey+y, (byte&0x08)?col_white:col_black);
			vo->set_pixel(basex+2, basey+y, (byte&0x10)?col_white:col_black);
			vo->set_pixel(basex+1, basey+y, (byte&0x20)?col_white:col_black);
			vo->set_pixel(basex  , basey+y, (byte&0x40)?col_white:col_black);
		}
		
		/*
		for (x=6; x>=0; x--) {
			if (byte&0x01) {
				if (dblwidth) {
					vo->set_pixel(basex+(x<<1),   basey+y, col_white);
					vo->set_pixel(basex+(x<<1)+1, basey+y, col_white);
				}
				else
					vo->set_pixel(basex+x, basey+y, col_white);
			} else {
				if (dblwidth) {
					vo->set_pixel(basex+(x<<1),   basey+y, col_black);
					vo->set_pixel(basex+(x<<1)+1, basey+y, col_black);
				}
				else
					vo->set_pixel(basex+x, basey+y, col_black);
			}	
			byte >>= 1;
		}*/
	}
}

void CTxt40ColRenderer::render(int startline, int endline) {
	int line, column, basex, basey, x, y;
    BYTE* chr;
	BYTE byte, mask;
	char_mode mode;
	bool flashon;
	
	timeval tv;
	gettimeofday(&tv,0);
	flashon = tv.tv_usec > 500000;
	
	if (charset) {
		basey = startline * 8;
		for (line=startline; line<=endline; line++) {
			basex = 0;
			for (column=0; column<40; column++) {
				byte = mem->read(GBASCALC(line) + column);
				renderChar(vo, byte, basex, basey, flashon, true);
				basex+=14;
			}
			basey+=8;
		}
	}
	else
		printf("No charset!!\n");
}


CTxt80ColRenderer::CTxt80ColRenderer(CMemory* mainMemory, CMemory* auxMemory, CVideoOutput* vo):CTextRenderer(mainMemory, vo) {
	this->aux = auxMemory;
}

void CTxt80ColRenderer::render(int startline, int endline) {
	int line, column, basex, basey, x, y;
    BYTE* chr;
	BYTE byte, mask;
	char_mode mode;
	bool flashon;
	
	timeval tv;
	gettimeofday(&tv,0);
	flashon = tv.tv_usec > 500000;
	
	if (charset) {
		basey = 0;
		for (line=startline; line<=endline; line++) {
			basex = 0;
			for (column=0; column<40; column++) {
				byte = aux->read(GBASCALC(line) + column - 1); // A verifier en 128K
				renderChar(vo, byte, basex, basey, flashon, false);
				basex+=7;
				byte = mem->read(GBASCALC(line) + column);
				renderChar(vo, byte, basex, basey, flashon, false);
				basex+=7;
			}
			basey+=8;
		}
	}
	else
		printf("No charset!!\n");
}


