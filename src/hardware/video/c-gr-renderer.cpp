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

#include "c-gr-renderer.hpp"

char shifted[4][16] = {
	{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
	{0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F},
	{0x00, 0x04, 0x08, 0x0C, 0x01, 0x05, 0x09, 0x0D, 0x02, 0x06, 0x0A, 0x0E, 0x03, 0x07, 0x0B, 0x0F},
	{0x00, 0x08, 0x01, 0x09, 0x02, 0x0A, 0x03, 0x0B, 0x04, 0x0C, 0x05, 0x0D, 0x06, 0x0E, 0x07, 0x0F}
};

CGrRenderer::CGrRenderer(CMemory* memory, CVideoOutput* vo, bool color):CVideoRenderer(memory, vo) {
	this->color = color;
}

void CGrRenderer::render_segment(int basex, int basey, char half) {
	if (color) {
		// In color, the whole 14 half pixels segment is set to the same color
		vo->set_pixel(basex   , basey, (apple_color)half);
		vo->set_pixel(basex+1 , basey, (apple_color)half);
		vo->set_pixel(basex+2 , basey, (apple_color)half);
		vo->set_pixel(basex+3 , basey, (apple_color)half);
		vo->set_pixel(basex+4 , basey, (apple_color)half);
		vo->set_pixel(basex+5 , basey, (apple_color)half);
		vo->set_pixel(basex+6 , basey, (apple_color)half);
		vo->set_pixel(basex+7 , basey, (apple_color)half);
		vo->set_pixel(basex+8 , basey, (apple_color)half);
		vo->set_pixel(basex+9 , basey, (apple_color)half);
		vo->set_pixel(basex+10, basey, (apple_color)half);
		vo->set_pixel(basex+11, basey, (apple_color)half);
		vo->set_pixel(basex+12, basey, (apple_color)half);
		vo->set_pixel(basex+13, basey, (apple_color)half);
	} else {
		// In B&W, the half pixels are set according to the bits coding the color
		half = shifted[basex&0x03][half]; // Shift bits according to X base position
		vo->set_pixel(basex   , basey, (half&0x01)?col_white:col_black);
		vo->set_pixel(basex+1 , basey, (half&0x02)?col_white:col_black);
		vo->set_pixel(basex+2 , basey, (half&0x04)?col_white:col_black);
		vo->set_pixel(basex+3 , basey, (half&0x08)?col_white:col_black);
		vo->set_pixel(basex+4 , basey, (half&0x01)?col_white:col_black);
		vo->set_pixel(basex+5 , basey, (half&0x02)?col_white:col_black);
		vo->set_pixel(basex+6 , basey, (half&0x04)?col_white:col_black);
		vo->set_pixel(basex+7 , basey, (half&0x08)?col_white:col_black);
		vo->set_pixel(basex+8 , basey, (half&0x01)?col_white:col_black);
		vo->set_pixel(basex+9 , basey, (half&0x02)?col_white:col_black);
		vo->set_pixel(basex+10, basey, (half&0x04)?col_white:col_black);
		vo->set_pixel(basex+11, basey, (half&0x08)?col_white:col_black);
		vo->set_pixel(basex+12, basey, (half&0x01)?col_white:col_black);
		vo->set_pixel(basex+13, basey, (half&0x02)?col_white:col_black);
	}
}

void CGrRenderer::render(int startline, int endline) {
	int line, column, basex, basey, x, y;
	BYTE byte, half;
	
	basey = startline * 8;
	for (line=startline; line<=endline; line++) {
		basex = 0;
		for (column=0; column<40; column++) {
			byte = mem->read(GBASCALC(line) + column);
			
			// A byte codes two big pixels of 4 pixel height
			half = byte & 0xf; // Fist big pixel
			render_segment(basex, basey  , half);
			render_segment(basex, basey+1, half);
			render_segment(basex, basey+2, half);
			render_segment(basex, basey+3, half);
			half = byte >> 4; // Second big pixel
			render_segment(basex, basey+4, half);
			render_segment(basex, basey+5, half);
			render_segment(basex, basey+6, half);
			render_segment(basex, basey+7, half);

			basex+=14;
		}
		basey += 8;
	}
}	
