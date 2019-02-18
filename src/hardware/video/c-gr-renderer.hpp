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

#ifndef _C_GR_RENDERER_HPP_
#define _C_GR_RENDERER_HPP_

#include "c-video-renderer.hpp"

class CGrRenderer: public CVideoRenderer
{
public:
	CGrRenderer(CMemory* memory, CVideoOutput* vo, bool color);
	void render(int startline, int endline);
protected:
	void render_segment(int basex, int basey, char half);
	bool color;
};

#endif // _C_GR_RENDERER_HPP_
