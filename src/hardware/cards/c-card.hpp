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

#ifndef _C_CARD_HPP_
#define _C_CARD_HPP_

#include "src/core/c-memory.hpp"
#include "src/core/units/c-unit.hpp"

class CCard
{
public:
	CCard();
	~CCard();
	CUnit *get_unit() { return unit; };
	CMemory *get_rom() { return rom; };
	CMemory *get_romExt() { return romExt; };
protected:
	CUnit *unit;
	CMemory *rom;
	CMemory *romExt;
private:
	CMemory *dummy;
};

#endif // _C_CARD_HPP_
