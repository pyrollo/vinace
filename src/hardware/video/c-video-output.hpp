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

// http://www.lazilong.com/apple_II/a2pix/dgr2.htm

#ifndef _C_VIDEO_OUTPUT_HPP_
#define _C_VIDEO_OUTPUT_HPP_

// Width is made of half pixels to manage :
// - pixel shifting in HGR mode
// - black and white GR mode
// - 80 column text mode
// 560 is 7(pixels)*40(columns)*2(half pixels)
#define VIDEO_OUTPUT_WIDTH 560
// Height is in plain pixels 
// 384 is 8(pixels)*24(lines)
#define VIDEO_OUTPUT_HEIGHT 192


const unsigned char color_r[16] = {0x00, 0xdd, 0x00, 0xdd, 0x00, 0x55, 0x22, 0x66, 0x88, 0xff, 0xaa, 0xff, 0x11, 0xff, 0x44, 0xff};
const unsigned char color_g[16] = {0x00, 0x00, 0x00, 0x22, 0x77, 0x55, 0x22, 0xaa, 0x55, 0x66, 0xaa, 0x99, 0xdd, 0xff, 0xff, 0xff};
const unsigned char color_b[16] = {0x00, 0x33, 0x99, 0xdd, 0x22, 0x55, 0xff, 0xff, 0x00, 0x00, 0xaa, 0x88, 0x00, 0x00, 0x99, 0xff};

const unsigned char bw_value[16] = {0x00, 0x5b, 0x33, 0x9f, 0x33, 0x55, 0x6c, 0xB0, 0x4a, 0x77, 0xaa, 0xb5, 0x4f, 0xaa, 0x9f, 0xff};


enum apple_color{
	col_black,        // HGR, TXT 
	col_deep_red,
	col_dark_blue,
	col_purple,       // HGR
	col_dark_green,
	col_dark_gray,
	col_medium_blue,  // HGR
	col_light_blue,
	col_brown,
	col_orange,       // HGR
	col_light_gray,
	col_pink,
	col_light_green,  // HGR
	col_yellow,
	col_aquamarine,
	col_white         // HGR, TXT
};

class CVideoOutput
{
public:
	CVideoOutput();
	virtual void render() = 0;
	char *get_pixels();
	void set_pixel (int x, int y, apple_color color) { lines[y][x] = color; };
	void set_dpixel(int x, int y, apple_color color) { lines[y][x] = color; lines[y][x+1] = color;};

protected:
	char pixels[VIDEO_OUTPUT_WIDTH*VIDEO_OUTPUT_HEIGHT]; // Actually only 4 lsb are significant
	char *lines[VIDEO_OUTPUT_HEIGHT];
};

#endif // _C_VIDEO_OUTPUT_HPP_
