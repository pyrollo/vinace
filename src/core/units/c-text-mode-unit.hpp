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

#ifndef _C_TEXT_MODE_UNIT_HPP_
#define _C_TEXT_MODE_UNIT_HPP_

#include "c-unit.hpp"

//
// Text Mode Unit
//
// This unit takes care 80 column and charset switches
//

class CTextModeUnit: public CUnit {
public:
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);

	FLAG_RO(80vid);   // true=80 column display, false=40 column display
	FLAG_RO(altchar); // true=alternative charset, false=primary chatset  
};


#endif // _C_TEXT_MODE_UNIT_HPP_
