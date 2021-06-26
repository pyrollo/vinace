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

#ifndef _C_MAIN_VIDEO_OUTPUT_HPP_
#define _C_MAIN_VIDEO_OUTPUT_HPP_

#include "c-text-mode-renderer.hpp"
#include "c-graphic-mode-renderer.hpp"
#include "core/c-memory.hpp"
#include "core/units/c-text-mode-unit.hpp"
#include "core/units/c-graphic-mode-unit.hpp"

class CMainVideoOutput: public CVideoOutput
{
public:
	CMainVideoOutput(CMemory *main, CMemory *aux, CTextModeUnit* textModeUnit, CGraphicModeUnit* graphicModeUnit, bool color);
	~CMainVideoOutput();
	void render();

protected:
	CGraphicModeUnit* graphicModeUnit;

	CGraphicModeRenderer *graphicModeRenderer;
	CTextModeRenderer    *textModeRenderer;
};

#endif // _C_MAIN_VIDEO_OUTPUT_HPP_
