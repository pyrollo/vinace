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

#ifndef _C_TEXT_MODE_RENDERER_HPP_
#define _C_TEXT_MODE_RENDERER_HPP_

#include "c-video-renderer.hpp"
#include "c-text-renderer.hpp"
#include "c-charset.hpp"
#include "src/core/units/c-text-mode-unit.hpp"
#include "src/core/units/c-graphic-mode-unit.hpp"


// TODO : Do like in graphic mode renderer with two items arrays for page1/page2
class CTextModeRenderer: public CVideoRenderer {
public:
	CTextModeRenderer(CMemory* main, CMemory* aux, 
	                  CTextModeUnit* textModeUnit, 
	                  CGraphicModeUnit* graphicModeUnit, 
	                  CVideoOutput* vo);
	~CTextModeRenderer();
	void render(int startline, int endline);
protected:
	// Managers
	CTextModeUnit    *textModeUnit;
	CGraphicModeUnit *graphicModeUnit;

	// Memory pages used for display
	CMemory *memtxt1;
	CMemory *memtxt2;
	CMemory *auxtxt1;
	CMemory *auxtxt2;

	// Text modes
	CTextRenderer *text40Page1;
	CTextRenderer *text40Page2;
	CTextRenderer *text80Page1;
	CTextRenderer *text80Page2;

	// Charsets
	CCharset *primaryCharset;
	CCharset *alternativeCharset;
};

#endif // _C_TEXT_MODE_RENDERER_HPP_
