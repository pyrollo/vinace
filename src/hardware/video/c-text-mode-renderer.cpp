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

#include "c-text-mode-renderer.hpp"

CTextModeRenderer::CTextModeRenderer(CMemory* main, CMemory* aux, CTextModeUnit* textModeUnit, CGraphicModeUnit* graphicModeUnit, CVideoOutput* vo)
	: CVideoRenderer(main, vo) {
	// Main members
	this->textModeUnit = textModeUnit;
	this->graphicModeUnit = graphicModeUnit; // Only for page2...

	// Memory pages used for display
	this->memtxt1 = new CMemoryProxy(main, 0x0400);
	this->memtxt2 = new CMemoryProxy(main, 0x0800);
	this->auxtxt1 = new CMemoryProxy(aux,  0x0400);
	this->auxtxt2 = new CMemoryProxy(aux,  0x0800);

	// Renderers for display modes
	this->text40Page1 = new CTxt40ColRenderer(memtxt1, vo);
	this->text40Page2 = new CTxt40ColRenderer(memtxt2, vo);
	this->text80Page1 = new CTxt80ColRenderer(memtxt1, auxtxt1, vo);
	this->text80Page2 = new CTxt80ColRenderer(memtxt2, auxtxt2, vo);

	// Charset
	this->primaryCharset = new C2ePrimaryCharset();
	this->alternativeCharset  = new C2eAlternativeCharset();
}

CTextModeRenderer::~CTextModeRenderer() {
	delete alternativeCharset;
	delete primaryCharset;
	delete text80Page2;
	delete text80Page1;
	delete text40Page2;
	delete text40Page1;
	delete auxtxt2;
	delete auxtxt1;
	delete memtxt2;
	delete memtxt1;
}

void CTextModeRenderer::render(int startline, int endline) {

	CTextRenderer *renderer;

	// TODO : could be better to use observers ?
	if (!textModeUnit) {
		renderer = (graphicModeUnit->get_page2())?text40Page2:text40Page1;
		renderer->set_charset(primaryCharset);
	} else {
		if (textModeUnit->get_80vid())
			renderer = (graphicModeUnit->get_page2())?text80Page2:text80Page1;
		else
			renderer = (graphicModeUnit->get_page2())?text40Page2:text40Page1;

		renderer->set_charset((textModeUnit->get_altchar())?alternativeCharset:primaryCharset);
	}
	renderer->render(startline, endline);
}



