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


#include "c-gui-color-monitor.hpp"

#define IMAGE_MONITOR "resources/colormon.jpg"

CGuiColorMonitor::CGuiColorMonitor(CVideoOutput *vo):CGuiMonitor(vo, IMAGE_MONITOR) {
}

inline void CGuiColorMonitor::get_color(char color, int &r, int &g, int &b) {
	r = color_b[color];
	g = color_g[color];
	b = color_r[color];
}

