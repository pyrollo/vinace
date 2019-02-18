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

#ifndef _C_UNIT_HPP_
#define _C_UNIT_HPP_

#include "../types.h"
#include "../c-memory.hpp"
#include "../c-observable.hpp"

// Helpers macros

#define FLAG_RO(flag) \
protected: \
	bool s_##flag; \
public: \
	bool get_##flag() { return s_##flag; }

#define FLAG_RW(flag) \
protected: \
	bool s_##flag; \
public: \
	bool get_ ##flag() { return s_##flag; } \
	void set_ ##flag(bool value) { s_##flag = value; notifyUpdate(); }

#define CASE_READ(flag, addr) \
	case addr: return bool_to_b7(s_##flag);

#define CASE_SWITCH(flag, addr0, addr1) \
	case addr0: s_##flag = false; break; \
	case addr1: s_##flag = true; break;

//
// CUnit : An abstract class for I/O Units
//

class CUnit: public CObservable
{
public:
	CUnit();
	virtual void reset() {};
	virtual BYTE read(BYTE addr) = 0;
	virtual void write(BYTE addr, BYTE byte) = 0;
};


#endif // _C_UNIT_HPP_
