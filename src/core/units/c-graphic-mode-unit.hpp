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

#ifndef _C_GRAPHIC_MODE_UNIT_HPP_
#define _C_GRAPHIC_MODE_UNIT_HPP_

#include "c-unit.hpp"

//
// Graphic Mode
//
// This unit takes care of graphic mode switches. It does not include
// DHGR switches  which should be in another unit.
//

class CGraphicModeUnit: public CUnit {
public:
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);

	FLAG_RO(text);
	FLAG_RO(mixed);
	FLAG_RO(page2);
	FLAG_RO(hires);

private:
    void access(BYTE addr);
};

#endif // _C_GRAPHIC_MODE_UNIT_HPP_
