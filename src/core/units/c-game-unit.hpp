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

#ifndef _C_GAME_UNIT_HPP_
#define _C_GAME_UNIT_HPP_

#include "c-unit.hpp"

//
// Game unit : Joystick/Paddles, buttons and outputs
//

class CGameUnit: public CUnit {
public:
	CGameUnit();
	void reset();
	void write(BYTE addr, BYTE byte);
	BYTE read(BYTE addr);

    FLAG_RW(pb0);
    FLAG_RW(pb1);
    FLAG_RW(pb2);
    FLAG_RO(an0);
    FLAG_RO(an1);
    FLAG_RO(an2);
    FLAG_RO(an3);
    FLAG_RW(paddl0);
    FLAG_RW(paddl1);
    FLAG_RW(paddl2);
    FLAG_RW(paddl3);
    FLAG_RO(ptrig);

private:
    void access(BYTE addr);
};

#endif // _C_GAME_UNIT_HPP_
