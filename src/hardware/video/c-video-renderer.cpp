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

#include "c-video-renderer.hpp"

CVideoRenderer::CVideoRenderer(CMemory *memory, CVideoOutput *vo) {
	this->vo  = vo;
	this->mem = memory;
}

// Render the whole screen
void CVideoRenderer::render_full() {
	render(0, 23);
}

// Render the upper part of the screen (for mixed mode)
void CVideoRenderer::render_top() {
	render(0, 19);
}

// Render the lower part of the screen (for mixed mode)
void CVideoRenderer::render_bottom() {
	render(20, 23);
}


