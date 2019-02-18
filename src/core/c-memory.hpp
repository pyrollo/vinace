/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _C_MEMORY_HPP_
#define _C_MEMORY_HPP_

#include "types.h"

//
// CMemory : Abstract memory class
//

class CMemory
{
public:
	virtual BYTE read(WORD) = 0;
	virtual void write(WORD, BYTE) {};
};

//
// CDummyMemory : A dummy memory that does not remember anything
//

class CDummyMemory : public CMemory
{
public:
	BYTE read(WORD) { return dummy_byte(); };
	void write(WORD, BYTE) {};
};

//
// CMemoryProxy : A subpart of a memory with addresses translation
//

class CMemoryProxy: public CMemory 
{
public:
	CMemoryProxy(CMemoryProxy *proxy, WORD start);
	CMemoryProxy(CMemory *memory, WORD start);

	BYTE read(WORD addr) { return memory->read(addr + start); }
	void write(WORD addr, BYTE byte) { memory->write(addr + start, byte); }
	
	// These methods are used when pipelining several proxies
	int get_start();
	CMemory *get_memory();

protected:
	int start;
	CMemory *memory;
};

//
// CRamMemory : Read / Write standard memory
//

class CRamMemory: public CMemory
{
public:
  	CRamMemory(int size);
    BYTE read(WORD addr) { return (addr < size)?buffer[addr]:dummy_byte(); };
    void write(WORD addr, BYTE byte) { if (addr < size) buffer[addr] = byte; };
    BYTE *get_buffer() { return buffer; }
	int get_size() { return size; }

protected:
	int size;
	BYTE *buffer;
};

//
// CRomMemory : Read only memory
//

class CRomMemory: public CRamMemory 
{
public:
  	CRomMemory(int size):CRamMemory(size) { };
    void write(WORD addr, BYTE byte) { };
};

#endif // _C_MEMORY_HPP_
