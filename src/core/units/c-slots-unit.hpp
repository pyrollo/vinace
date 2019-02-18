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

#ifndef _C_SLOTS_UNIT_HPP_
#define _C_SLOTS_UNIT_HPP_

#include "c-unit.hpp"

//
// Slots
//

class CSlotsUnit: public CUnit {
public:
	CSlotsUnit();
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);

	void insert_card(int slot, CUnit *cardUnit);
	void remove_card(int slot);

private:
	CUnit *slots[7];
};

#endif // _C_SLOTS_UNIT_HPP_
