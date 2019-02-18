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


#include "c-graphic-mode-renderer.hpp"
#include "c-gr-renderer.hpp"
#include "c-hgr-renderer.hpp"

CGraphicModeRenderer::CGraphicModeRenderer(CMemory* memory, CGraphicModeUnit* graphicModeUnit, CVideoOutput *vo, bool color)
	: CVideoRenderer(memory, vo) {
	// Main members
	this->graphicModeUnit = graphicModeUnit;

	// Memory pages used for display
	this->memgr[0]  = new CMemoryProxy(memory, 0x0400);
	this->memgr[1]  = new CMemoryProxy(memory, 0x0800);
	this->memhgr[0] = new CMemoryProxy(memory, 0x2000);
	this->memhgr[1] = new CMemoryProxy(memory, 0x4000);
	
	// Renderers for display modes
	gr[0]  = new CGrRenderer(memgr[0], vo, color);
	gr[1]  = new CGrRenderer(memgr[1], vo, color);
	hgr[0] = new CHgrRenderer(memhgr[0], vo, color);
	hgr[1] = new CHgrRenderer(memhgr[1], vo, color);
}

CGraphicModeRenderer::~CGraphicModeRenderer() {
	delete hgr[1];
	delete hgr[0];
	delete gr[1];
	delete gr[0];
	delete memhgr[1];
	delete memhgr[0];
	delete memgr[1];
	delete memgr[0];
}

void CGraphicModeRenderer::render(int startline, int endline) {
	CVideoRenderer *renderer;

	renderer = (graphicModeUnit->get_hires())?
		hgr[graphicModeUnit->get_page2()]:
		gr[graphicModeUnit->get_page2()];

	renderer->render(startline, endline);
};


