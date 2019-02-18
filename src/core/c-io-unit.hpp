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

#ifndef _C_IO_UNIT_HPP_
#define _C_IO_UNIT_HPP_
#include "c-memory.hpp"
#include "units/c-unit.hpp"

class CDummyUnit:public CUnit
{
public:
	void write(BYTE addr, BYTE byte) {};
	BYTE read(BYTE addr) { return dummy_byte() & 0x7F; };
};

class CIoUnit: public CMemory 
{
public:
	CIoUnit();
	virtual void reset() = 0;
	virtual void write(WORD addr, BYTE byte);
	virtual BYTE read(WORD addr);
 
	void assign_read(BYTE addr, CUnit *unit);
	void assign_write(BYTE addr, CUnit *unit);
	void assign_read_write(BYTE addr, CUnit *unit);
	void assign_read(BYTE startaddr, BYTE endaddr, CUnit *unit);
	void assign_write(BYTE startaddr, BYTE endaddr, CUnit *unit);
	void assign_read_write(BYTE startaddr, BYTE endaddr, CUnit *unit);

protected:
	CUnit *dummy;
	CUnit *read_map[256];
	CUnit *write_map[256];
};

#endif // _C_IO_UNIT_HPP_
