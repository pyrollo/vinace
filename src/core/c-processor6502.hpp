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


#ifndef _C_PROCESSOR6502_HPP_
#define _C_PROCESSOR6502_HPP_

#include "types.h"
#include "c-processor.hpp"
#include "c-memory.hpp"

// Signals
#define NUMBER_OF_SIGNALS 4
#define SIGNAL_RESET 0 // Reset signal
#define SIGNAL_NMI   1 // Non masquable interrupt signal
#define SIGNAL_IRQ   2 // Interrupt request signal
#define SIGNAL_BRK   3 // Break signal

// State register bits
#define N_BIT	0X80
#define V_BIT	0X40
#define B_BIT	0x10
#define D_BIT	0x08
#define I_BIT	0x04
#define Z_BIT	0x02
#define C_BIT	0x01

class CProcessor6502: public CProcessor 
{
public:
	CProcessor6502(CMemory *memory);
	void cycle(); // Play one machine cycle (may do nothing)

	// Direct access to registers
	WORD get_PC() { return PC; };
	BYTE get_A()  { return A;  };
	BYTE get_X()  { return X;  };
	BYTE get_Y()  { return Y;  };
	BYTE get_P()  { return P;  };
	BYTE get_S()  { return S;  };

	// Access to last executed instruction address
	WORD get_last_PC() { return last_PC; };


protected:

	// Registers
	WORD PC;
	BYTE A;
	BYTE X;
	BYTE Y;
	BYTE P;
	BYTE S;
	
	// Last processed instruction address
	WORD last_PC;
	
	// Mandatory CProcessor virtual methods
	virtual void reset(); // Reset the processor
	virtual void process_instruction(); // Execute the next opcode
	virtual bool process_signals(); // Process signals, returns true if no signal and false if there was one
	

	// Memory methods
    void write_byte(WORD addr, BYTE byte);
    BYTE read_byte(WORD addr);
	WORD read_word(WORD addr);
	BYTE next_byte();
	WORD next_word();
	
	// Stack
	void push(BYTE byte);
    BYTE pull();
    void push_word(WORD word);
    WORD pull_word();
	
	// Status register
	void set_p(short bit, bool state);
    int get_p(short bit);

	// Pseudo decimal mode
    WORD bcdadjustadd(WORD result);
    WORD bcdadjustsub(WORD result);	

	// Addresses methods
	void check_page_crossing(WORD addr, int offset);

    WORD eazp(BYTE offset=0);
    WORD eaabs(BYTE offset=0, bool extracycle = true);
    WORD eazpxind();
    WORD eazpindy(bool extracycle = true);

    // Factorized code
    void branch(BYTE operand);
    void testresult(WORD result);
    void testvalue(BYTE value);
    void opadd(BYTE operand);
    void opsub(BYTE operand);
    void oprol(WORD addr, bool rotate);
    void opror(WORD addr, bool rotate);
    void opcmp(BYTE operand1, BYTE operand2);
	void opdec(WORD addr);
	void opinc(WORD addr);

private:

	// Some variables put here for optimisation purpose (avoid memory reservation once proc started)
	BYTE opcode;
	BYTE operand;
	BYTE byte;
	WORD word;
	WORD addr;

};


#endif // _C_PROCESSOR6502_HPP_
