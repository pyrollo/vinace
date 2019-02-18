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

#ifndef _C_IO_ROM_UNIT_HPP_
#define _C_IO_ROM_UNIT_HPP_

#include "c-unit.hpp"

//
// Input Output Rom Unit
//
// This unit takes care of $C100-$CFFF memory management switches
//
// See "Apple IIe Technical Reference Manual" page 142

class CIoRomUnit: public CUnit {
public:
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);

	FLAG_RO(cxrom); // true=slots, false=internal
	FLAG_RO(c3rom); // true=slot3, false=internal

private:
    void access(BYTE addr);
};

#endif // _C_IO_ROM_UNIT_HPP_
